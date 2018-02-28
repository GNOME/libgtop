/**
*Move the DBus to examples in Libgtop
*/
#include <glibtop/stats.h>
#include <glib.h>
#include <gio/gio.h>
#include <stdlib.h>
#include <stdio.h>

static const gchar netstats_introspection_xml[] =
"<node name='/org/gnome/GTop/NetStats'>"
"  <interface name='org.gnome.GTop.NetStats'>"
"    <method name='GetStats'>"
"      <arg type='a{i(dd)}' name='pid' direction='out'/>"
"    </method>"
"    <method name='init_capture'>"
"    </method>"
"  </interface>"
"</node>";
    
static GDBusNodeInfo *netstats_data = NULL;
static guint owner_id;
static GMainLoop *loop;
//source_id
guint id;    
static void
handle_method_call (GDBusConnection       *connection,
                    const gchar           *sender,
                    const gchar           *object_path,
                    const gchar           *method_name,
                    GVariant              *parameters,
                    GDBusMethodInvocation *invocation,
                    gpointer               user_data)
{ 
    GPtrArray *temp = (stats*) user_data;
    if (g_strcmp0 (method_name, "GetStats") == 0)
    {

        GVariantBuilder *b;
        GVariant *dict;
        b = g_variant_builder_new (G_VARIANT_TYPE ("(a{i(dd)})"));
        g_variant_builder_open (b, G_VARIANT_TYPE ("a{i(dd)}"));
        guint num_proc = temp->len;
        while(num_proc--)
        {
            stats* dbus_stats = g_ptr_array_index(temp,num_proc);
            g_variant_builder_add (b, 
                                  "{i(dd)}",
                                   dbus_stats->pid,
                                   g_variant_new ("(dd)",
                                   dbus_stats->bytes_sent,
                                   dbus_stats->bytes_recv));

        }
        g_variant_builder_close (b);
        dict = g_variant_builder_end (b);
        g_dbus_method_invocation_return_value (invocation,
                                                dict);
    }

    if (g_strcmp0 (method_name, "init_capture") == 0)
    {
       id = g_timeout_add(5000,glibtop_init_packet_capture,NULL);
       g_dbus_method_invocation_return_value (invocation,
                                               NULL);
    }
}
     
static const GDBusInterfaceVTable interface_vtable =
{
    handle_method_call,
    NULL,
    NULL
};
     
static void
on_netstats_bus_acquired (GDBusConnection *connection,
                          const gchar     *name,
                          gpointer         user_data)
{
    GError *error = NULL;
    guint registration_id;

    registration_id = g_dbus_connection_register_object (connection,
                                                        "/org/gnome/GTop/netstats",
                                                        netstats_data->interfaces[0],
                                                        &interface_vtable,
                                                        NULL,
                                                        NULL,
                                                        &error);
    if (registration_id == 0)
    {
        g_warning ("Failed to register object: %s\n", error->message);
        g_error_free (error);
    }
}

static void
on_name_acquired (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
}
     
static void
on_name_lost (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
    exit(0);
}
     
int
main (int argc, char *argv[])
{
    GError *err = NULL;
    g_type_init ();
    GPtrArray *user_stats = get_stats_instance(NULL);
    
    netstats_data = g_dbus_node_info_new_for_xml (netstats_introspection_xml, &err);
    if (netstats_data == NULL)
    {
        g_warning ("Error parsing introspection XML: %s\n", err->message);
        g_error_free (err);
        goto error;
    }
    owner_id = g_bus_own_name (G_BUS_TYPE_SESSION,
                                "org.gnome.GTop.NetStats",
                                G_BUS_NAME_OWNER_FLAGS_NONE, //check
                                on_netstats_bus_acquired,
                                on_name_acquired,
                                on_name_lost,
                                user_stats,
                                NULL);

    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);
    if (owner_id > 0)
    {
        g_bus_unown_name (owner_id);
        owner_id = 0;
    }
    g_dbus_node_info_unref (netstats_data);
error:
    if (netstats_data)
        g_dbus_node_info_unref (netstats_data);
return 0;
}
