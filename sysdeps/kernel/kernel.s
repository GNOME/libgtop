	.file	"test.c"
	.version	"01.01"
/ GNU C version egcs-2.90.29 980515 (egcs-1.0.3 release) (i686-pc-linux-gnu) compiled by GNU C version egcs-2.90.29 980515 (egcs-1.0.3 release).
/ options passed:  -O2 -Wall -W
/ options enabled:  -fdefer-pop -fcse-follow-jumps -fcse-skip-blocks
/ -fexpensive-optimizations -fthread-jumps -fstrength-reduce -fpeephole
/ -fforce-mem -ffunction-cse -finline -fkeep-static-consts -fcaller-saves
/ -fpcc-struct-return -frerun-cse-after-loop -frerun-loop-opt
/ -fschedule-insns2 -fcommon -fverbose-asm -fgnu-linker -fregmove
/ -falias-check -fargument-alias -m80387 -mhard-float -mno-soft-float
/ -mieee-fp -mfp-ret-in-387 -mschedule-prologue -mcpu=pentiumpro
/ -march=pentium

gcc2_compiled.:
.text
	.align 4
.globl table
	.type	 table,@function
table:
	pushl %ebp
	movl %esp,%ebp
	movl $190,%eax
	pushl %ebx
	movl 8(%ebp),%ebx
	movl 12(%ebp),%ecx
	movl 16(%ebp),%edx
#APP
	int $0x80
#NO_APP
	testl %eax,%eax
	jge .L3
	negl %eax
	movl %eax,errno
	movl $-1,%eax
.L3:
	movl -4(%ebp),%ebx
	movl %ebp,%esp
	popl %ebp
	ret
.Lfe1:
	.size	 table,.Lfe1-table
	.ident	"GCC: (GNU) egcs-2.90.29 980515 (egcs-1.0.3 release)"
