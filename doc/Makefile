all:	.timestamp .timestamp2

.timestamp:	libgtop.sgml
	rm -rf libgtop
	mkdir libgtop
	jade -D /usr/lib/sgml/jade_dsl -d libgtop.dsl -t sgml \
		-V %no-make-index% libgtop.sgml > /dev/null && \
		touch .timestamp

.timestamp2:	gnome-hackers.sgml
	rm -rf gnome-hackers
	mkdir gnome-hackers
	jade -D /usr/lib/sgml/jade_dsl -d gnome-hackers.dsl -t sgml \
		-V %no-make-index% gnome-hackers.sgml > /dev/null && \
		touch .timestamp2
