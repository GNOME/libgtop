LINK = $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(AM_LDFLAGS) -o $@

noinst_PROGRAMS = gtop-dbus-service

AM_CPPFLAGS= ${GTOP_DBUS_CFLAGS}

gtop_dbus_service_SOURCES = gtop-dbus-service.c
gtop_dbus_service_LDADD = $(GTOP_DBUS_LIBS)
