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
}

/^(\w+)/	{
  features[$1] = $1;
  if ($1 ~ /^proclist$/) {
    output = "SCM\nglibtop_guile_get_proclist (void)\n{\n";
    output = output"\tglibtop_proclist proclist;\n\tunsigned i, *ptr;\n";
    output = output"\tSCM list;\n\n\tptr = glibtop_get_proclist (&proclist);\n\n";
    output = output"\tlist = gh_list (gh_ulong2scm  ("$1".flags),\n\t\t\t";
  } else {
    if ($1 ~ /^proc_/) {
      output = "SCM\nglibtop_guile_get_"$1" (SCM pid)\n{\n";
      output = output"\tglibtop_"$1" "$1";\n\n";
      output = output"\tglibtop_get_"$1" (&"$1", (pid_t) gh_scm2long (pid));\n\n";
    } else {
      output = "SCM\nglibtop_guile_get_"$1" (void)\n{\n";
      output = output"\tglibtop_"$1" "$1";\n\n";
      output = output"\tglibtop_get_"$1" (&"$1");\n\n";
    }
    output = output"\treturn gh_list (gh_ulong2scm  ("$1".flags),\n\t\t\t";
  }
  nr_elements = split ($2, elements, /:/);
  for (element = 1; element <= nr_elements; element++) {
    list = elements[element];
    type = elements[element];
    sub(/\(.*/, "", type);
    sub(/^\w+\(/, "", list); sub(/\)$/, "", list);
    count = split (list, fields, /,/);
    for (field = 1; field <= count; field++) {
      output = output""convert[type]" ("$1"."fields[field]"),\n\t\t\t";
    }
  }
  output = output"SCM_UNDEFINED);";
  print output;

  if ($1 ~ /^proclist$/) {
    print "";
    print "\tif (ptr) {";
    print "\t\tfor (i = 0; i < proclist.number; i++)";
    print "\t\t\tlist = gh_append";
    print "\t\t\t\t(gh_list (list,";
    print "\t\t\t\t\t  gh_list (gh_ulong2scm ((unsigned long) ptr [i])),";
    print "\t\t\t\t\t  SCM_UNDEFINED));";
    print "\t}";
    print "";
    print "\tglibtop_free (ptr);";
    print "";
    print "\treturn list;";
  }
  print "}";
  print "";
}

END {
  print "void";
  print "glibtop_boot_guile (void)";
  print "{";

  for (feature in features) {
    if (feature ~ /^proc_/) {
      print "\tgh_new_procedure1_0";
    } else {
      print "\tgh_new_procedure0_0";
    }
    print "\t\t(\"glibtop-get-"feature"\", glibtop_guile_get_"feature");";
  }
  print "}";
}

