servicedir = /usr/share/dbus-1/services
dbusservicedir = /etc/dbus-1/system.d/
service_in_files = org.gnome.GTop.NetStats.service.in
dbusservice_DATA = org.gnome.GTop.NetStats.conf
service_DATA = $(service_in_files:.service.in=.service)
$(service_DATA): $(service_in_files) Makefile
	$(AM_V_GEN) $(SED) -e "s|\@libexecdir\@|$(libexecdir)|" $< > $@ 
EXTRA_DIST = \
	$(service_in_files) \
	$(dbusservice_DATA)

