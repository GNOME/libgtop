BEGIN {
  print "/* lib.c */";
  print "/* This is a generated file.  Please modify `lib.awk' */";
  print "";

  print "#include <glibtop.h>";
  print "#include <glibtop/sysdeps.h>";
  print "#include <glibtop/command.h>";

  print "";

}

function output(feature) {
  if (feature ~ /^proclist$/) {
    print "unsigned *";
    prefix = "return ";
  } else {
    prefix = "";
    print "void";
  }
  if (feature ~ /^proc_/) {
    param = ", pid_t pid";
  } else {
    param = "";
  }

  print "glibtop_get_"feature"_l (glibtop *server, glibtop_"feature" *buf"param")";
  print "{";
  print "\tglibtop_init_r (&server, GLIBTOP_SYSDEPS_"toupper(feature)", 0);";
  print "";
  print "\tif (server->features & GLIBTOP_SYSDEPS_"toupper(feature)") {";

  if (feature ~ /^proc_/) {
    print "\t\t"prefix"glibtop_call_l (server, GLIBTOP_CMND_"toupper(feature)", sizeof (pid_t),";
    print "\t\t\t\t&pid, sizeof (glibtop_"feature"), buf);";
    print "\t} else {";
    print "\t\t"prefix"glibtop_get_"feature"_r (server, buf, pid);";
  } else {
    print "\t\t"prefix"glibtop_call_l (server, GLIBTOP_CMND_"toupper(feature)", 0, NULL,";
    print "\t\t\t  sizeof (glibtop_"feature"), buf);";
    print "\t} else {";
    print "\t\t"prefix"glibtop_get_"feature"_r (server, buf);";
  }
  print "\t}";
  print "}";
  print "";
}

/^(\w+)/	{ output($1) }


