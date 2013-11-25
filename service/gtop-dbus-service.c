#include <gio/gio.h>
#include <stdlib.h>
#include <stdio.h>
#include <config.h>
#include <glib.h>

#include "gtop-dbus-service.h"

// required for renice
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>

static const gchar object_name[] = "/org/gnome/gtopServer";
static const gchar processes_object_name[] = "/org/gnome/gtopServer/Processes";

#define VERBOSE 1
/* ---------------------------------------------------------------------------------------------------- */

static GDBusNodeInfo *introspection_data = NULL;

/* Introspection data for the service we are exporting */
static const gchar introspection_xml[] =
  "<node name='/org/gnome/gtopServer'>"
  "  <interface name='org.gnome.gtop'>"
  "    <property type='s' name='Version' access='read'/>"
  "  </interface>"
  "  <interface name='org.gnome.gtop.Processes'>"
  "    <method name='SendSignal'>"
  "      <arg name='pid' type='i' direction='in' />"
  "      <arg name='signal' type='i' direction='in' />"
  "    </method>"
  "    <method name='Renice'>"
  "      <arg name='pid' type='i' direction='in' />"
  "      <arg name='nice' type='i' direction='in' />"
  "    </method>"
  "    <method name='GetProperties'>"
  "      <arg name='pid' type='i' direction='in' />"
  "      <arg name='propertyList' type='as' direction='in' />"
  "      <arg name='properties' type='a{sv}' direction='out' />"
  "    </method>"
  "  </interface>"
  "</node>";

/** 
 * Handle a call to a method.  (Parameters should be obvious.)
 */
static void
handle_method_call (GDBusConnection       *connection,
                    const gchar           *sender,
                    const gchar           *object_path,
                    const gchar           *interface_name,
                    const gchar           *method_name,
                    GVariant              *parameters,
                    GDBusMethodInvocation *invocation,
                    gpointer               user_data)
{
#ifdef VERBOSE
    gchar *paramstr = g_variant_print (parameters, TRUE);
    fprintf (stderr, MSG_PREFIX
             "handle_method_call (%p,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",(invocation),%p)\n",
             connection, sender, object_path, interface_name, method_name, 
             paramstr, user_data);
    g_free (paramstr);
#endif
    if (g_strcmp0 (method_name, "Renice") == 0)
    {
        int pid, prio;
        g_variant_get (parameters, "(ii)", &pid, &prio);
        gint renice_result = setpriority (PRIO_PROCESS, pid, CLAMP(prio, PRIO_MIN, PRIO_MAX));
#ifdef VERBOSE
        fprintf (stderr, MSG_PREFIX"renicing '%d' to '%d' priority returned %d", pid, prio, renice_result);
#endif      
        if (renice_result == 0) 
            g_dbus_method_invocation_return_value (invocation, NULL);
        else {
            switch (errno) {
                case ESRCH: g_dbus_method_invocation_return_error (invocation, 
                                                                   G_IO_ERROR,
                                                                   G_IO_ERROR_NOT_FOUND,
                                                                   MSG_PREFIX "Process '%d' not found",
                                                                   pid);
                break;
                case EACCES:
                case EPERM: g_dbus_method_invocation_return_error (invocation, 
                                                                   G_IO_ERROR,
                                                                   G_IO_ERROR_PERMISSION_DENIED,
                                                                   MSG_PREFIX "Permission denied to change priority of process '%d' to '%d'",
                                                                   pid, prio);
                break;
            }
        }
            
    }

    // Anything else is an error.
    else
    {
  // Default: No such method
      g_dbus_method_invocation_return_error (invocation,
                                             G_IO_ERROR,
                                             G_IO_ERROR_INVALID_ARGUMENT,
                                             MSG_PREFIX "Invalid method: '%s' on '%s'",
                                             method_name, interface_name);
    }
} // handle_method_call

/**
 * Handle a request to set a property.
 */
static gboolean
handle_set_property (GDBusConnection  *connection,
                     const gchar      *sender,
                     const gchar      *object_path,
                     const gchar      *interface_name,
                     const gchar      *property_name,
                     GVariant         *value,
                     GError          **error,
                     gpointer          user_data)
{
    // Print an optional log message
#ifdef VERBOSE
    gchar *valstr = g_variant_print (value, TRUE);
    fprintf (stderr, MSG_PREFIX
             "handle_set_property (%p,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",(error),%p)\n",
             connection, sender, object_path, interface_name, property_name, 
             valstr, user_data);
    g_free (valstr);
#endif

    g_set_error (error,
                 G_IO_ERROR,
                 G_IO_ERROR_FAILED,
                 MSG_PREFIX "No such property: '%s'",
                 property_name);
    return 0;
} // handle_set_property

/**
 * Handle a request for a property.  (Parameters should be obvious.)
 */
static GVariant *
handle_get_property (GDBusConnection  *connection,
                     const gchar      *sender,
                     const gchar      *object_path,
                     const gchar      *interface_name,
                     const gchar      *property_name,
                     GError          **error,
                     gpointer          user_data)
{
  // Print an optional log message
#ifdef VERBOSE
    fprintf (stderr, MSG_PREFIX
             "handle_get_property (%p,\"%s\",\"%s\",\"%s\",\"%s\",(error),%p)\n",
             connection, sender, object_path, interface_name, property_name, 
             user_data);
#endif

    if (g_strcmp0 (property_name, "Version") == 0)
    {
        return g_variant_new_string (LIBGTOP_VERSION);
    }

    // Anything else is an error.
    else
    {
        g_set_error (error,
                     G_IO_ERROR,
                     G_IO_ERROR_FAILED,
                     MSG_PREFIX "Invalid property '%s'",
                     property_name);
        return NULL;
    } // unknown property
} // handle_get_property

/**
 * What to do when the bus gets acquired.
 */
static void
on_bus_acquired (GDBusConnection *connection,
                 const gchar     *name,
                 gpointer        user_data)
{
    static GDBusInterfaceVTable interface_vtable =
      {
        handle_method_call,
        handle_get_property,
        handle_set_property
      };

    guint registration_id;
    GError *error = NULL;

  // A bit of (optional) notification
#ifdef VERBOSE
    fprintf (stderr, MSG_PREFIX "on_bus_acquired (%p, \"%s\", %p)\n", 
             connection, name, user_data);
#endif

    registration_id = 
      g_dbus_connection_register_object (connection,
                                         object_name,
                                         introspection_data->interfaces[0],
                                         &interface_vtable,
                                         NULL,    // Optional user data
                                         NULL,    // Func. for freeing user data
                                         &error);
                                         
                                         

    registration_id = 
      g_dbus_connection_register_object (connection,
                                         processes_object_name,
                                         introspection_data->interfaces[1],
                                         &interface_vtable,
                                         NULL,    // Optional user data
                                         NULL,    // Func. for freeing user data
                                         &error);
                                         
} // on_bus_acquired

static void
on_name_acquired (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
  // A bit of (optional) notification
#ifdef VERBOSE
    fprintf (stderr, MSG_PREFIX "on_name_acquired (%p, \"%s\", %p)\n", 
             connection, name, user_data);
#endif
} // on_name_acquired

static void
on_name_lost (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
  // A bit of (optional) notification
#ifdef VERBOSE
    fprintf (stderr, MSG_PREFIX "on_name_lost (%p, \"%s\", %p)\n", 
             connection, name, user_data);
#endif
  // Things seem to have gone badly wrong, so give up
    exit (1);
} // on_name_lost

int main ( int argc, char ** argv ) {
    guint owner_id;
    GMainLoop *loop;
    
    introspection_data = g_dbus_node_info_new_for_xml (introspection_xml, NULL);
    
    owner_id = g_bus_own_name (G_BUS_TYPE_SYSTEM,
                               GTOP_SERVER,
                               G_BUS_NAME_OWNER_FLAGS_NONE,
                               on_bus_acquired,
                               on_name_acquired,
                               on_name_lost,
                               NULL,
                               NULL);
                               
    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);
    
    // Tell the bus that we're done with the name
    g_bus_unown_name (owner_id);

    // Clean up after ourselves
    g_dbus_node_info_unref (introspection_data);
  
    return 0;
}
