all:	.timestamp

.timestamp:	libgtop.sgml
	jade -D /usr/lib/sgml/jade_dsl -d libgtop.dsl -t sgml \
		-V %no-make-index% libgtop.sgml > /dev/null 
	touch .timestamp
