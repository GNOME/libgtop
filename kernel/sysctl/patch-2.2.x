diff -ru kernel-source-2.2.12/Makefile hacker-2.2.12/Makefile
--- kernel-source-2.2.12/Makefile	Thu Aug 26 02:29:45 1999
+++ hacker-2.2.12/Makefile	Sat Dec 18 17:33:04 1999
@@ -113,6 +113,7 @@
 DRIVERS		=drivers/block/block.a \
 		 drivers/char/char.a \
 	         drivers/misc/misc.a
+EXTRAS		=
 LIBS		=$(TOPDIR)/lib/lib.a
 SUBDIRS		=kernel drivers mm fs net ipc lib
 
@@ -202,6 +203,11 @@
 DRIVERS := $(DRIVERS) drivers/net/irda/irda_drivers.a
 endif
 
+ifdef CONFIG_LIBGTOP
+SUBDIRS := $(SUBDIRS) libgtop
+EXTRAS  := $(EXTRAS) libgtop/kernel.o
+endif
+
 include arch/$(ARCH)/Makefile
 
 .S.s:
@@ -222,6 +228,7 @@
 		$(FILESYSTEMS) \
 		$(NETWORKS) \
 		$(DRIVERS) \
+		$(EXTRAS) \
 		$(LIBS) \
 		--end-group \
 		-o vmlinux
diff -ru kernel-source-2.2.12/arch/i386/config.in hacker-2.2.12/arch/i386/config.in
--- kernel-source-2.2.12/arch/i386/config.in	Thu Oct 14 00:57:46 1999
+++ hacker-2.2.12/arch/i386/config.in	Sat Dec 18 17:33:04 1999
@@ -88,6 +88,9 @@
 bool 'System V IPC' CONFIG_SYSVIPC
 bool 'BSD Process Accounting' CONFIG_BSD_PROCESS_ACCT
 bool 'Sysctl support' CONFIG_SYSCTL
+if [ "$CONFIG_EXPERIMENTAL" = "y" ]; then
+  tristate 'LibGTop support' CONFIG_LIBGTOP
+fi
 tristate 'Kernel support for a.out binaries' CONFIG_BINFMT_AOUT
 tristate 'Kernel support for ELF binaries' CONFIG_BINFMT_ELF
 tristate 'Kernel support for MISC binaries' CONFIG_BINFMT_MISC
diff -ru kernel-source-2.2.12/include/linux/autoconf.h hacker-2.2.12/include/linux/autoconf.h
--- kernel-source-2.2.12/include/linux/autoconf.h	Sat Dec  4 01:20:30 1999
+++ hacker-2.2.12/include/linux/autoconf.h	Sat Dec 18 17:41:44 1999
@@ -53,6 +53,8 @@
 #define CONFIG_SYSVIPC 1
 #define CONFIG_BSD_PROCESS_ACCT 1
 #define CONFIG_SYSCTL 1
+#undef  CONFIG_LIBGTOP
+#define CONFIG_LIBGTOP_MODULE 1
 #undef  CONFIG_BINFMT_AOUT
 #define CONFIG_BINFMT_AOUT_MODULE 1
 #define CONFIG_BINFMT_ELF 1
diff -ru kernel-source-2.2.12/include/linux/sysctl.h hacker-2.2.12/include/linux/sysctl.h
--- kernel-source-2.2.12/include/linux/sysctl.h	Mon Aug  9 21:05:13 1999
+++ hacker-2.2.12/include/linux/sysctl.h	Sat Dec 18 17:34:38 1999
@@ -57,7 +57,8 @@
 	CTL_FS=5,		/* Filesystems */
 	CTL_DEBUG=6,		/* Debugging */
 	CTL_DEV=7,		/* Devices */
-	CTL_BUS=8		/* Buses */
+	CTL_BUS=8,		/* Buses */
+	CTL_LIBGTOP=408		/* LibGTop */
 };
 
 /* CTL_BUS names: */
diff -ru kernel-source-2.2.12/kernel/sysctl.c hacker-2.2.12/kernel/sysctl.c
--- kernel-source-2.2.12/kernel/sysctl.c	Mon Aug  9 21:05:13 1999
+++ hacker-2.2.12/kernel/sysctl.c	Sat Dec 18 17:33:04 1999
@@ -83,7 +83,9 @@
 static ctl_table fs_table[];
 static ctl_table debug_table[];
 static ctl_table dev_table[];
-
+#ifdef CONFIG_LIBGTOP
+extern ctl_table libgtop_table[];
+#endif
 
 /* /proc declarations: */
 
@@ -149,6 +151,9 @@
 	{CTL_FS, "fs", NULL, 0, 0555, fs_table},
 	{CTL_DEBUG, "debug", NULL, 0, 0555, debug_table},
         {CTL_DEV, "dev", NULL, 0, 0555, dev_table},
+#ifdef CONFIG_LIBGTOP
+	{CTL_LIBGTOP, "libgtop", NULL, 0, 0555, libgtop_table},
+#endif
 	{0}
 };
 
