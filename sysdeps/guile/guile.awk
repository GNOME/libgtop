BEGIN {
  print "/* guile.c */";
  print "/* This is a generated file.  Please modify `guile.awk' */";
  print "";

  print "#include <glibtop.h>";
  print "#include <glibtop/xmalloc.h>";
  print "#include <glibtop/sysdeps.h>";
  print "#include <glibtop/union.h>";
  print "";
  print "#include <guile/gh.h>";

  print "";

  convert["long"]   = "gh_long2scm  ";
  convert["ulong"]  = "gh_ulong2scm ";
  convert["double"] = "gh_double2scm";
  convert["str"]    = "gh_str02scm  ";
  convert["char"]   = "gh_char2scm  ";

  backconv["int"]          = "gh_scm2long";
  backconv["pid_t"]        = "gh_scm2ulong";
  backconv["long"]         = "gh_scm2long";
  backconv["ulong"]        = "gh_scm2ulong";
}

function make_output(line) {
  split (line, line_fields, /\|/);
  retval = line_fields[1];
  element_def = line_fields[3];
  feature = line_fields[2];
  param_def = line_fields[4];

  sub(/^@/,"",feature);
  features[feature] = feature;

  total_nr_params = 0;

  if (param_def == "string") {
    call_param = ", gh_scm2newstr( "line_fields[5]", NULL)";
    param_decl = "SCM "line_fields[5];
    total_nr_params = 1;
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
      total_nr_params = total_nr_params + count;
      for (field = 1; field <= count; field++) {
	if (param_decl != "")
	  param_decl = param_decl", ";
	param_decl = param_decl"SCM "fields[field];
	call_param = call_param", "backconv[type]" ("fields[field]")";
      }
    }
    if (param_decl == "")
      param_decl = "void";
  }

  nr_params_field[feature] = total_nr_params;

  feature_name = feature; sub(/_/,"-",feature_name);
  output = "SCM_PROC (s_"feature", \"glibtop-get-"feature_name"\",";
  output = output" "nr_params_field[feature]", 0, 0, ";
  output = output"glibtop_guile_get_"feature");\n\n";
  
  output = output"SCM\nglibtop_guile_get_"feature" ("param_decl")\n{\n";

  output = output"\tglibtop_"feature" "feature";\n";
  if (retval != "void")
    output = output"\t"retval" retval;\n";
  if (feature ~ /^proclist$/)
    output = output"\tunsigned i;\n";
  output = output"\tSCM list;\n\n";
  
  if (retval != "void")
    prefix="retval = ";
  else
    prefix="";

  output = output"\t"prefix"glibtop_get_"feature" (&"feature""call_param");\n\n";
  
  output = output"\tlist = gh_list (gh_ulong2scm  ("feature".flags),\n\t\t\t";

  nr_elements = split (element_def, elements, /:/);
  for (element = 1; element <= nr_elements; element++) {
    list = elements[element];
    type = elements[element];
    sub(/\(.*/, "", type);
    sub(/^.*\(/, "", list); sub(/\)$/, "", list);
    count = split (list, fields, /,/);
    for (field = 1; field <= count; field++) {
      output = output""convert[type]" ("feature"."fields[field]"),\n\t\t\t";
    }
  }
  output = output"SCM_UNDEFINED);\n";

  print output;

  if (feature ~ /^proclist$/) {
    print "\tif (retval == NULL)";
    print "\t\treturn list;";
    print "";
    print "\tfor (i = 0; i < proclist.number; i++)";
    print "\t\tlist = scm_append";
    print "\t\t\t(gh_list (list,";
    print "\t\t\t\t  gh_list (gh_ulong2scm ((unsigned long) retval [i])),";
    print "\t\t\t\t  SCM_UNDEFINED));";
    print "";
    print "\tglibtop_free (retval);\n";
  }

  print "\treturn list;";
  print "}";
  print "";
}

/^[^#]/		{ make_output($0) }

END {
  print "void";
  print "glibtop_boot_guile (void)";
  print "{";
  print "fprintf (stderr, \"glibtop_boot_guile ()\\n\");";
  print "#include \"guile.x\"";
  print "}";
}
