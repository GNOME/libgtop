BEGIN {
  print "/* Libgtop.xs */";
  print "/* This is a generated file.  Please modify `perl.awk' */";
  print "";

  print "#ifdef __cplusplus";
  print "extern \"C\" {";
  print "#endif";
  print "#include \"EXTERN.h\"";
  print "#include \"perl.h\"";
  print "#include \"XSUB.h\"";
  print "#ifdef __cplusplus";
  print "}";
  print "#endif";
  print "";
  print "#undef PACKAGE";
  print "";
  print "#include <glibtop.h>";
  print "#include <glibtop/union.h>";
  print "#include <glibtop/xmalloc.h>";
  print "";
  print "MODULE = Libgtop\t\tPACKAGE = Libgtop";
  print "";
  print "PROTOTYPES: ENABLE";
  print "";
  print "glibtop *";
  print "init (CLASS)";
  print "\tchar *CLASS;";
  print "CODE:";
  print "\tRETVAL = glibtop_calloc (1, sizeof (glibtop));";
  print "\tglibtop_init_s (&RETVAL, 0, 0);";
  print "OUTPUT:";
  print "\tRETVAL";
  print "";

  type_convert["long"]   = "int64_t";
  type_convert["ulong"]  = "u_int64_t";
  type_convert["pid_t"]  = "pid_t";
  type_convert["int"]    = "int";

  convert["long"]   = "newSViv";
  convert["ulong"]  = "newSViv";
  convert["double"] = "newSVnv";
}

function output(line) {
  split (line, line_fields, /\|/);
  retval = line_fields[1];
  feature = line_fields[2];
  element_def = line_fields[3];
  param_def = line_fields[4];

  orig = feature; sub(/^@/,"",feature);
  space = feature; gsub(/./," ",space);

  if (feature ~ /^proc_map/)
    return;
  else if (feature ~ /^mountlist/)
    return;
  else if (feature ~ /^fsusage/)
    return;

  if (param_def == "string") {
    call_param = line_fields[5];
    param_decl = "\tconst char *"line_fields[5]";\n";
  } else {
    call_param = "";
    param_decl = "";
    nr_params = split (param_def, params, /:/);
    for (param = 1; param <= nr_params; param++) {
      list = params[param];
      type = params[param];
      sub(/\(.*/, "", type);
      sub(/^.*\(/, "", list); sub(/\)$/, "", list);
      count = split (list, fields, /,/);
      for (field = 1; field <= count; field++) {
	param_decl = param_decl"\t";
	call_param = call_param", ";
	param_decl = param_decl""type_convert[type]" "fields[field]";\n";
	call_param = call_param""fields[field];
      }
    }
  }
  
  print "void";
  print feature" (server"call_param")";
  print "\tglibtop *server;";
  print param_decl"PREINIT:";
  print "\tglibtop_"feature" "feature";";

  if (feature ~ /^proclist/) {
    print "\tunsigned i, *ptr;";
    print "PPCODE:";
    print "\tptr = glibtop_get_proclist (&proclist, which, arg);";
    print "";
    print "\tif (ptr) {";
    print "\t\tfor (i = 0; i < proclist.number; i++)";
    print "\t\t\tXPUSHs (sv_2mortal (newSViv (ptr [i])));";
    print "\t}";
    print "";
    print "\tglibtop_free (ptr);";
  } else {
    print "PPCODE:";
    if (call_param != "")
      print "\tglibtop_get_"feature"_l (server, &"feature""call_param");";
    else
      print "\tglibtop_get_"feature"_l (server, &"feature");";
    print "";
    
    nr_elements = split (element_def, elements, /:/);
    for (element = 1; element <= nr_elements; element++) {
      list = elements[element];
      type = elements[element];
      sub(/\(.*/, "", type);
      sub(/^.*\(/, "", list); sub(/\)$/, "", list);
      count = split (list, fields, /,/);
      for (field = 1; field <= count; field++) {
	if (type ~ /^str$/) {
	  print "\tXPUSHs (sv_2mortal (newSVpv ("feature"."fields[field]", 0)));";
	} else {
	  if (type ~ /^char$/) {
	    print "\tXPUSHs (sv_2mortal (newSVpv (&"feature"."fields[field]", 1)));";
	  } else {
	    print "\tXPUSHs (sv_2mortal ("convert[type]" ("feature"."fields[field]")));";
	  }
	}
      }
    }
  }
    
  print "";
}

/^[^#]/		{ output($0) }

