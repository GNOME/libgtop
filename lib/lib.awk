BEGIN {
  print "/* lib.c */";
  print "/* This is a generated file.  Please modify `lib.awk' */";
  print "";
  
  print "#include <glibtop.h>";
  print "#include <glibtop/open.h>";
  print "";
  print "#include <glibtop/sysdeps.h>";
  print "#include <glibtop/union.h>";
  print "";
  print "#include <glibtop/command.h>";
  
  print "";
  print "/* Some required fields are missing. */";
  print "";

  print "static void";
  print "_glibtop_missing_feature (glibtop *server, const char *feature,";
  print "\t\t\t  const u_int64_t present, u_int64_t *required)";
  print "{";
  print "\tswitch (server->error_method) {";
  print "\tcase GLIBTOP_ERROR_METHOD_WARN_ONCE:";
  print "\t\t*required &= present;";
  print "\tcase GLIBTOP_ERROR_METHOD_WARN:";
  print "\t\tglibtop_warn_r (server,";
  print "\t\t\t\t_(\"glibtop_get_%s (): Client requested \"";
  print "\t\t\t\t  \"field mask %05Lx, but only have %05Lx.\"),";
  print "\t\t\t\t feature, required, present);";
  print "\t\tbreak;";
  print "\tcase GLIBTOP_ERROR_METHOD_ABORT:";
  print "\t\tglibtop_error_r (server,";
  print "\t\t\t\t _(\"glibtop_get_%s (): Client requested \"";
  print "\t\t\t\t   \"field mask %05x, but only have %05x.\"),";
  print "\t\t\t\t feature, required, present);";
  print "\t\tbreak;";
  print "\t}";
  print "}";

  print "";
  print "/* Library functions. */";
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
    prefix = "retval = ";
    prefix_space = "         ";
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
  if (retval !~ /^void$/)
    print "\t"retval" retval;\n";
  print "\tglibtop_init_r (&server, (1 << GLIBTOP_SYSDEPS_"toupper(feature)"), 0);";

  print "";
  print "\t/* If neccessary, we ask the server for the requested";
  print "\t * feature. If not, we call the sysdeps function. */";
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
    print "\t\t"prefix"glibtop_get_"feature"_s (server, buf);";
  else
    print "\t\t"prefix"glibtop_get_"feature"_s (server, buf, "param");";

  if (orig !~ /^@/) {
    print "#else";
    print "\t\terrno = ENOSYS;";
    print "\t\tglibtop_error_io_r (server, \"glibtop_get_"feature"\");";
    print "#endif";
  }

  print "\t}";

  print "";
  print "\t/* Make sure that all required fields are present. */";
  print "";

  print "\tif (buf->flags & server->required."feature")";
  print "\t\t_glibtop_missing_feature (server, \""feature"\", buf->flags,";
  print "\t\t\t\t\t  &server->required."feature");";

  if (retval !~ /^void$/) {
    print "\n\t/* Now we can return. */";
    print "\n\treturn retval;";
  }

  print "}";
  print "";
}
	
/^[^#]/		{ output($0) }

