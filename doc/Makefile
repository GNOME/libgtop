all:	.timestamp .timestamp2 .timestamp3 .timestamp4

clean:
	-rm -f .timestamp*
	-rm -rf libgtop gnome-hackers libgtop-ref table

.timestamp:	libgtop.sgml
	-rm -rf libgtop
	mkdir libgtop
	-rm -f .timestamp
	jade -D /usr/lib/sgml -d libgtop.dsl -t sgml \
		-V %no-make-index% libgtop.sgml > /dev/null && \
		touch .timestamp

.timestamp2:	gnome-hackers.sgml autoconf.sgml ../guile/reference.sgml
	-rm -rf gnome-hackers
	mkdir gnome-hackers
	-rm -f .timestamp2
	jade -D /usr/lib/sgml -d gnome-hackers.dsl -t sgml \
		-V %no-make-index% gnome-hackers.sgml > /dev/null && \
		touch .timestamp2

.timestamp3:	libgtop-ref.sgml ../guile/reference.sgml
	-rm -rf libgtop-ref
	mkdir libgtop-ref
	-rm -f .timestamp3
	jade -D /usr/lib/sgml -d libgtop-ref.dsl -t sgml \
		-V %no-make-index% libgtop-ref.sgml > /dev/null && \
		touch .timestamp3

.timestamp4:	table.sgml
	-rm -rf table
	mkdir table
	-rm -f .timestamp4
	jade -D /usr/lib/sgml -d table.dsl -t sgml \
		-V %no-make-index% table.sgml > /dev/null && \
		touch .timestamp4
