htmldir		= $(prefix)/html

html_subdirs	= libgtop gnome-hackers libgtop-ref table

# Well, yes - you are using GNU Make, aren't you ... ?
stamp_FILES	= $(addsuffix .stamp,$(html_subdirs))
sgml_FILES	= $(addsuffix .sgml,$(html_subdirs))
dsl_FILES	= $(addsuffix .dsl,$(html_subdirs))

EXTRA_DIST	= $(sgml_FILES) $(dsl_FILES) dbtohtml.dsl autoconf.sgml

noinst_DATA	= $(stamp_FILES)

CLEANFILES	= $(stamp_FILES)

distclean-local:
	-rm -rf $(html_subdirs)

libgtop.sgml: libgtopConf.sh

gnome-hackers.sgml: autoconf.sgml $(top_builddir)/guile/reference.sgml \
	features/uptime.sgml features/uptime.txt features/proclist.sgml \
	features/procmem.sgml

libgtopConf.sh:
	@LN_S@ `@LIBGTOP_CONFIG@ --config`

# You can set the $(JADE_FLAGS) in your shell to pass additional
# arguments like `-D /usr/lib/sgml' to jade.

SUFFIXES = .stamp .sgml
.sgml.stamp:
	echo rm -f $@
	-rm -rf $*
	mkdir $*
	jade $(JADE_FLAGS) -D $(srcdir) -D . -D $(top_builddir)/guile \
		-d $(srcdir)/$*.dsl -t sgml -V %no-make-index% $< \
		> /dev/null && touch $@

install-data-local:
	$(mkinstalldirs) $(htmldir)
	for subdir in $(html_subdirs) ; do \
	  $(mkinstalldirs) $(htmldir)/$$subdir ; \
	  for file in $$subdir/* ; do \
	    $(INSTALL_DATA) $$file $(htmldir)/$$subdir ; \
	  done \
	done

dist-hook:
	mkdir $(distdir)/features
	cp -p $(srcdir)/features/*.sgml $(distdir)/features
	cp -p $(srcdir)/features/*.txt $(distdir)/features

