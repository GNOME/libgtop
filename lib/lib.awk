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

  convert["long"]   = "int64_t";
  convert["ulong"]  = "u_int64_t";
  convert["pid_t"]  = "pid_t";
  convert["int"]    = "int";
}

function output(line) {
  split (line, line_fields, /\|/);
  retval = line_fields[1];
  feature = line_fields[2];
  param_def = line_fields[4];

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

  if (param_def == "string") {
    call_param = ", "line_fields[5];
    param_decl = ",\n            "space"    const char *"line_fields[5];
    send_ptr = "\n\tvoid *send_ptr = "line_fields[5]";";
    send_size = "\n\tconst size_t send_size =\n\t\tstrlen ("line_fields[5]") + 1;";
  } else {
    call_param = "";
    param_decl = "";
    send_size = "";
    send_ptr = "";
    nr_params = split (param_def, params, /:/);
    for (param = 1; param <= nr_params; param++) {
      list = params[param];
      type = params[param];
      sub(/\(.*/, "", type);
      sub(/^\w+\(/, "", list); sub(/\)$/, "", list);
      count = split (list, fields, /,/);
      for (field = 1; field <= count; field++) {
	if (param_decl == "")
	  param_decl = ",\n            "space"    ";
	else
	  param_decl = param_decl", ";
	param_decl = param_decl""convert[type]" "fields[field];
	call_param = call_param", "fields[field];
	if (send_ptr == "")
	  send_ptr = "\n\tvoid *send_ptr = &"fields[field]";";
	if (send_size == "")
	  send_size = "\n\tconst size_t send_size =\n\t\t";
	else
	  send_size = send_size" + ";
	send_size = send_size"sizeof ("fields[field]")";
      }
    }
    if (send_size != "")
      send_size = send_size";";
    else
      send_size = "\n\tconst size_t send_size = 0;";
    if (send_ptr == "")
      send_ptr = "\n\tvoid *send_ptr = NULL;";
  }

  print "glibtop_get_"feature"_l (glibtop *server, glibtop_"feature" *buf"param_decl")";
  
  print "{"send_ptr""send_size;
  if (retval !~ /^void$/)
    print "\t"retval" retval;";
  print "";

  print "\tglibtop_init_r (&server, (1 << GLIBTOP_SYSDEPS_"toupper(feature)"), 0);";

  print "";
  print "\t/* If neccessary, we ask the server for the requested";
  print "\t * feature. If not, we call the sysdeps function. */";
  print "";

  print "\tif ((server->flags & _GLIBTOP_INIT_STATE_SERVER) &&";
  print "\t    (server->features & (1 << GLIBTOP_SYSDEPS_"toupper(feature)")))";
  print "\t{";

  print "\t\t"prefix"glibtop_call_l (server, GLIBTOP_CMND_"toupper(feature)",";
  print "\t\t\t\t"prefix_space"send_size, send_ptr,";
  print "\t\t\t\t"prefix_space"sizeof (glibtop_"feature"), buf);";
  
  print "\t} else {";

  if (orig !~ /^@/)
    print "#if (!GLIBTOP_SUID_"toupper(feature)")";

  print "\t\t"prefix"glibtop_get_"feature"_s (server, buf"call_param");";

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

