BEGIN {
  print "/* guile.c */";
  print "/* This is a generated file.  Please modify `guile.awk' */";
  print "";

  print "#include <glibtop.h>";
  print "#include <glibtop/xmalloc.h>";
  print "#include <glibtop/sysdeps.h>";
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
}

function make_output(line) {
  split (line, line_fields, /\|/);
  retval = line_fields[1];
  element_def = line_fields[3];
  feature = line_fields[2];
  param_typ = line_fields[4];
  param = line_fields[5];
  param_size = line_fields[6];

  sub(/^@/,"",feature);
  features[feature] = feature;

  if (param == "")
    output = "SCM\nglibtop_guile_get_"feature" (void)\n{\n";
  else
    output = "SCM\nglibtop_guile_get_"feature" (SCM "param")\n{\n";

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

  if (param_typ == "const char *")
    param_conv = "gh_scm2newstr ("param", NULL)";
  else if (param_typ != "")
    param_conv = backconv[param_typ]" ("param")";

  if (param == "")
    output = output"\t"prefix"glibtop_get_"feature" (&"feature");\n\n";
  else
    output = output"\t"prefix"glibtop_get_"feature" (&"feature", "param_conv");\n\n";
  
  output = output"\tlist = gh_list (gh_ulong2scm  ("feature".flags),\n\t\t\t";

  nr_elements = split (element_def, elements, /:/);
  for (element = 1; element <= nr_elements; element++) {
    list = elements[element];
    type = elements[element];
    sub(/\(.*/, "", type);
    sub(/^\w+\(/, "", list); sub(/\)$/, "", list);
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

  for (feature in features) {
    if (feature ~ /^proc_/) {
      print "\tgh_new_procedure1_0";
    } else if (feature ~ /^fsusage$/) {
      print "\tgh_new_procedure1_0";
    } else {
      print "\tgh_new_procedure0_0";
    }
    print "\t\t(\"glibtop-get-"feature"\", glibtop_guile_get_"feature");";
  }
  print "}";
}

