BEGIN {
  print "/* lib.c */";
  print "/* This is a generated file.  Please modify `lib.awk' */";
  print "";
  
  print "#include <glibtop.h>";
  print "#include <glibtop/open.h>";
  print "#include <glibtop/sysdeps.h>";
  print "#include <glibtop/command.h>";
  
  print "";
}

function output(feature) {
  orig = feature;
  sub(/@/,"",feature);
  if (feature ~ /^proclist$/) {
    print "unsigned *";
    prefix = "return ";
  } else if (feature ~ /^mountlist$/) {
    print "glibtop_mountentry *";
    prefix = "return ";
  } else {
    prefix = "";
    print "void";
  }
  if (feature ~ /^proc_/) {
    param = ", pid_t pid";
  } else if (feature ~ /^fsusage$/) {
    param = ", const char *mountdir";
  } else if (feature ~ /^mountlist$/) {
    param = ", int all_fs";
  } else {
    param = "";
  }
  
  print "glibtop_get_"feature"_l (glibtop *server, glibtop_"feature" *buf"param")";
  print "{";
  print "\tglibtop_init_r (&server, GLIBTOP_SYSDEPS_"toupper(feature)", 0);";
  print "";
  print "\tif ((server->flags & _GLIBTOP_INIT_STATE_SERVER) &&";
  print "\t    (server->features & (1 << GLIBTOP_SYSDEPS_"toupper(feature)")))";
  print "\t{";
  
  if (feature ~ /^proc_/) {
    print "\t\tglibtop_call_l (server, GLIBTOP_CMND_"toupper(feature)",";
    print "\t\t\t\tsizeof (pid_t), &pid,";
    print "\t\t\t\tsizeof (glibtop_"feature"),";
    print "\t\t\t\tbuf);";
    print "\t} else {";
    print "#if (!GLIBTOP_SUID_"toupper(feature)")";
    print "\t\tglibtop_get_"feature"_r (server, buf, pid);";
  } else if (feature ~ /^fsusage$/) {
    print "\t\tglibtop_call_l (server, GLIBTOP_CMND_"toupper(feature)",";
    print "\t\t\t\tstrlen (mountdir) + 1, mountdir,";
    print "\t\t\t\tsizeof (glibtop_"feature"), buf);";
    print "\t} else {";
    print "#if (!GLIBTOP_SUID_"toupper(feature)")";
    print "\t\tglibtop_get_"feature"_r (server, buf, mountdir);";
  } else {
    if (feature ~ /^mountlist$/) {
      print "\t\treturn glibtop_call_l (server, GLIBTOP_CMND_MOUNTLIST,";
      print "\t\t\t\t       sizeof (all_fs), &all_fs,";
      print "\t\t\t\t       sizeof (glibtop_mountlist),";
      print "\t\t\t\t       buf);";
    } else if (feature ~ /^proclist$/) {
      print "\t\treturn glibtop_call_l (server, GLIBTOP_CMND_PROCLIST,";
      print "\t\t\t\t       0, NULL, sizeof (glibtop_proclist),";
      print "\t\t\t\t       buf);";
    } else {
      print "\t\tglibtop_call_l (server, GLIBTOP_CMND_"toupper(feature)", 0, NULL,";
      print "\t\t\t\tsizeof (glibtop_"feature"), buf);";
    }
    print "\t} else {";
    if (orig ~ /^@/) {
      if (feature ~ /^mountlist$/) {
	print "\t\t"prefix"glibtop_get_"feature"_r (server, buf, all_fs);";
      } else {
	print "\t\t"prefix"glibtop_get_"feature"_s (server, buf);";
      }
    } else {
      print "#if (!GLIBTOP_SUID_"toupper(feature)")";
      print "\t\t"prefix"glibtop_get_"feature"_r (server, buf);";
    }
  }
  if (!(orig ~ /^@/)) {
    print "#else";
    print "\t\terrno = ENOSYS;";
    print "\t\tglibtop_error_io_r (server, \"glibtop_get_"feature"\");";
    print "#endif";
  }
  print "\t}";
  print "}";
  print "";
}
	
/^@(\w+)/	{ output($1) }

/^(\w+)/	{ output($1) }

