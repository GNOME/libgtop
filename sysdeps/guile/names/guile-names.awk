BEGIN {
  print "/* guile_names.c */";
  print "/* This is a generated file.  Please modify `guile-names.awk' */";
  print "";

  print "#include <glibtop.h>";
  print "#include <glibtop/sysdeps.h>";
  print "";
  print "#include <guile/gh.h>";

  print "";

}

function output(feature) {
  print "SCM";
  print "glibtop_guile_names_"feature" (void)";
  print "{";
  print "\tint i;";
  print "\tSCM list;";
  print "";
  print "\tlist = gh_list (SCM_UNDEFINED);";
  print "";
  print "\tfor (i = 0; i < GLIBTOP_MAX_"toupper(feature)"; i++)";
  print "\t\tlist = gh_append";
  print "\t\t\t(gh_list (list,";
  print "\t\t\t\t  gh_list (gh_str02scm ((char *) glibtop_names_"feature" [i])),";
  print "\t\t\t\t  SCM_UNDEFINED));";
  print "";
  print "\treturn list;";
  print "}";
  
  print "";
  
  print "SCM";
  print "glibtop_guile_types_"feature" (void)";
  print "{";
  print "\tint i;";
  print "\tSCM list;";
  print "";
  print "\tlist = gh_list (SCM_UNDEFINED);";
  print "";
  print "\tfor (i = 0; i < GLIBTOP_MAX_"toupper(feature)"; i++)";
  print "\t\tlist = gh_append";
  print "\t\t\t(gh_list (list,";
  print "\t\t\t\t  gh_list (gh_ulong2scm (glibtop_types_"feature" [i])),";
  print "\t\t\t\t  SCM_UNDEFINED));";
  print "";
  print "\treturn list;";
  print "}";
  
  print "";
  
  print "SCM";
  print "glibtop_guile_labels_"feature" (void)";
  print "{";
  print "\tint i;";
  print "\tSCM list;";
  print "";
  print "\tlist = gh_list (SCM_UNDEFINED);";
  print "";
  print "\tfor (i = 0; i < GLIBTOP_MAX_"toupper(feature)"; i++)";
  print "\t\tlist = gh_append";
  print "\t\t\t(gh_list (list,";
  print "\t\t\t\t  gh_list (gh_str02scm (gettext";
  print "\t\t\t\t\t\t\t(glibtop_labels_"feature" [i]))),";
  print "\t\t\t\t  SCM_UNDEFINED));";
  print "";
  print "\treturn list;";
  print "}";
  
  print "";
  
  print "SCM";
  print "glibtop_guile_descriptions_"feature" (void)";
  print "{";
  print "\tint i;";
  print "\tSCM list;";
  print "";
  print "\tlist = gh_list (SCM_UNDEFINED);";
  print "";
  print "\tfor (i = 0; i < GLIBTOP_MAX_"toupper(feature)"; i++)";
  print "\t\tlist = gh_append";
  print "\t\t\t(gh_list (list,";
  print "\t\t\t\t  gh_list (gh_str02scm (gettext";
  print "\t\t\t\t\t\t\t(glibtop_descriptions_"feature" [i]))),";
  print "\t\t\t\t  SCM_UNDEFINED));";
  print "";
  print "\treturn list;";
  print "}";

  print "";
}

/^(\w+)/	{ features[$1] = $1 }

END {
  for (feature in features) {
    output(feature);
  }

  print "void";
  print "glibtop_boot_guile_names (void)";
  print "{";

  for (feature in features) {
    print "\tgh_new_procedure0_0";
    print "\t\t(\"glibtop-names-"feature"\", glibtop_guile_names_"feature");";
    print "";
    print "\tgh_new_procedure0_0";
    print "\t\t(\"glibtop-types-"feature"\", glibtop_guile_types_"feature");";
    print "";
    print "\tgh_new_procedure0_0";
    print "\t\t(\"glibtop-labels-"feature"\", glibtop_guile_labels_"feature");";
    print "";
    print "\tgh_new_procedure0_0";
    print "\t\t(\"glibtop-descriptions-"feature"\", glibtop_guile_descriptions_"feature");";
  }
  print "}";
}

