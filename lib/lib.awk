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

function output(line) {
  split (line, line_fields, /\|/);
  retval = line_fields[1];
  feature = line_fields[2];
  param_typ = line_fields[4];
  param = line_fields[5];
  param_size = line_fields[6];

  if (param_typ == "") {
    param_size = "0";
    param_ptr = "NULL";
  } else {
    if (param_size == "")
      param_size = "sizeof ("param_typ")";

    if (param_typ ~ /*/)
      param_ptr = param;
    else
      param_ptr = "&"param;
  }

  orig = feature; sub(/^@/,"",feature);
  space = feature; gsub(/./," ",space);

  print retval;
  if (retval !~ /^void$/) {
    prefix = "return ";
    prefix_space = "       ";
  } else {
    prefix = "";
    prefix_space = "";
  }

  if (param_typ != "") {
    print "glibtop_get_"feature"_l (glibtop *server, glibtop_"feature" *buf,";
    print "            "space"    "param_typ" "param")";
  } else {
    print "glibtop_get_"feature"_l (glibtop *server, glibtop_"feature" *buf)";
  }

  print "{";
  print "\tglibtop_init_r (&server, GLIBTOP_SYSDEPS_"toupper(feature)", 0);";
  print "";
  print "\tif ((server->flags & _GLIBTOP_INIT_STATE_SERVER) &&";
  print "\t    (server->features & (1 << GLIBTOP_SYSDEPS_"toupper(feature)")))";
  print "\t{";

  if (param == "")
    print "\t\t"prefix"glibtop_call_l (server, GLIBTOP_CMND_"toupper(feature)", 0, NULL,";
  else
    print "\t\t"prefix"glibtop_call_l (server, GLIBTOP_CMND_"toupper(feature)",";

  if (param == "") {
    print "\t\t\t\t"prefix_space"sizeof (glibtop_"feature"), buf);";
  } else {
    print "\t\t\t\t"prefix_space""param_size", "param_ptr",";
    print "\t\t\t\t"prefix_space"sizeof (glibtop_"feature"),";
    print "\t\t\t\t"prefix_space"buf);";
  }
  
  print "\t} else {";

  if (orig !~ /^@/)
    print "#if (!GLIBTOP_SUID_"toupper(feature)")";

  if (param == "")
    print "\t\t"prefix"glibtop_get_"feature"_r (server, buf);";
  else
    print "\t\t"prefix"glibtop_get_"feature"_r (server, buf, "param");";

  if (orig !~ /^@/) {
    print "#else";
    print "\t\terrno = ENOSYS;";
    print "\t\tglibtop_error_io_r (server, \"glibtop_get_"feature"\");";
    print "#endif";
  }

  print "\t}";
  print "}";
  print "";
}
	
/^[^#]/		{ output($0) }

