info_TEXINFOS		= libgtop.texi

libgtop_TEXINFOS	= libgtop.texi about.texi reference.texi \
			  auto-macros.texi version.texi main.texi \
			  white-paper.texi

MAKEINFO		+= -I @libgtop_top_builddir@/doc

EXTRA_DIST		= auto-macros.texi.in

auto-macros.texi: auto-macros.texi.in Makefile
## Use sed and then mv to avoid problems if the user interrupts.
	sed -e 's#\%LIBGTOP_LIBDIR\%#$(libdir)#g' \
	  -e 's#\%LIBGTOP_INCLUDEDIR\%#$(includedir)#g' \
	  -e 's#\%LIBGTOP_DATADIR\%#$(datadir)#g' \
	  -e 's#\%LIBGTOP_EXTRA_LIBS\%#$(LIBGTOP_EXTRA_LIBS)#g' \
	  -e 's#\%LIBGTOP_LIBS\%#$(LIBGTOP_LIBS)#g' \
	  -e 's#\%LIBGTOP_INCS\%#$(LIBGTOP_INCS)#g' \
	  -e 's#\%LIBGTOP_NAMES_LIBS\%#$(LIBGTOP_NAMES_LIBS)#g' \
	  -e 's#\%LIBGTOP_NAMES_INCS\%#$(LIBGTOP_NAMES_INCS)#g' \
	  -e 's#\%LIBGTOP_GUILE_LIBS\%#$(LIBGTOP_GUILE_LIBS)#g' \
	  -e 's#\%LIBGTOP_GUILE_INCS\%#$(LIBGTOP_GUILE_INCS)#g' \
	  -e 's#\%LIBGTOP_GUILE_NAMES_LIBS\%#$(LIBGTOP_GUILE_NAMES_LIBS)#g' \
	  -e 's#\%LIBGTOP_GUILE_NAMES_INCS\%#$(LIBGTOP_GUILE_NAMES_INCS)#g' \
	  -e 's#\%LIBGTOP_BINDIR\%#$(LIBGTOP_BINDIR)#g' \
	  -e 's#\%LIBGTOP_SERVER\%#$(LIBGTOP_SERVER)#g' \
	  -e 's#\%LIBGTOP_MAJOR_VERSION\%#$(LIBGTOP_MAJOR_VERSION)#g' \
	  -e 's#\%LIBGTOP_MINOR_VERSION\%#$(LIBGTOP_MINOR_VERSION)#g' \
	  -e 's#\%LIBGTOP_MICRO_VERSION\%#$(LIBGTOP_MICRO_VERSION)#g' \
	  -e 's#\%LIBGTOP_VERSION\%#$(LIBGTOP_VERSION)#g' \
	  -e 's#\%LIBGTOP_VERSION_CODE\%#$(LIBGTOP_VERSION_CODE)#g' \
	  -e 's#\%LIBGTOP_SERVER_VERSION\%#$(LIBGTOP_SERVER_VERSION)#g' \
	  -e 's#\%libgtop_sysdeps_dir\%#$(libgtop_sysdeps_dir)#g' \
	  -e 's#\%libgtop_need_server\%#$(libgtop_need_server)#g' \
	  -e 's#\%libgtop_use_machine_h\%#$(libgtop_use_machine_h)#g' \
	  -e 's#\%libgtop_guile_found\%#$(libgtop_guile_found)#g' \
	  -e 's#\%libgtop_want_examples\%#$(libgtop_want_examples)#g' \
	      < $(srcdir)/auto-macros.texi.in > auto-macros.tmp
	echo '@c Set this if this is LibGTop 1.1.x' >> auto-macros.tmp
	if [ $(LIBGTOP_VERSION_CODE) -ge 1001000 ] ; then \
	  echo '@set LIBGTOP-1-1' >> auto-macros.tmp ; \
	else \
	  echo '@clear LIBGTOP-1-1' >> auto-macros.tmp ; \
	fi
	mv auto-macros.tmp auto-macros.texi
