BEGIN {
  print "/* guile_names.c */";
  print "/* This is a generated file.  Please modify `guile-names.awk' */";
  print "";

  print "#include <glibtop.h>";
  print "#include <glibtop/sysdeps.h>";
  print "#include <glibtop/union.h>";
  print "";
  print "#include <guile/gh.h>";

  print "";

}

function output(feature) {
  print "static SCM";
  print "glibtop_guile_names_"feature" (void)";
  print "{";
  print "\tint i;";
  print "\tSCM list;";
  print "";
  print "\tlist = gh_list (SCM_UNDEFINED);";
  print "";
  print "\tfor (i = 0; i < GLIBTOP_MAX_"toupper(feature)"; i++)";
  print "\t\tlist = scm_append";
  print "\t\t\t(gh_list (list,";
  print "\t\t\t\t  gh_list (gh_str02scm ((char *) glibtop_names_"feature" [i])),";
  print "\t\t\t\t  SCM_UNDEFINED));";
  print "";
  print "\treturn list;";
  print "}";
  
  print "";
  
  print "static SCM";
  print "glibtop_guile_types_"feature" (void)";
  print "{";
  print "\tint i;";
  print "\tSCM list;";
  print "";
  print "\tlist = gh_list (SCM_UNDEFINED);";
  print "";
  print "\tfor (i = 0; i < GLIBTOP_MAX_"toupper(feature)"; i++)";
  print "\t\tlist = scm_append";
  print "\t\t\t(gh_list (list,";
  print "\t\t\t\t  gh_list (gh_ulong2scm (glibtop_types_"feature" [i])),";
  print "\t\t\t\t  SCM_UNDEFINED));";
  print "";
  print "\treturn list;";
  print "}";
  
  print "";
  
  print "static SCM";
  print "glibtop_guile_labels_"feature" (void)";
  print "{";
  print "\tint i;";
  print "\tSCM list;";
  print "";
  print "\tlist = gh_list (SCM_UNDEFINED);";
  print "";
  print "\tfor (i = 0; i < GLIBTOP_MAX_"toupper(feature)"; i++)";
  print "\t\tlist = scm_append";
  print "\t\t\t(gh_list (list,";
  print "\t\t\t\t  gh_list (gh_str02scm (gettext";
  print "\t\t\t\t\t\t\t(glibtop_labels_"feature" [i]))),";
  print "\t\t\t\t  SCM_UNDEFINED));";
  print "";
  print "\treturn list;";
  print "}";
  
  print "";
  
  print "static SCM";
  print "glibtop_guile_descriptions_"feature" (void)";
  print "{";
  print "\tint i;";
  print "\tSCM list;";
  print "";
  print "\tlist = gh_list (SCM_UNDEFINED);";
  print "";
  print "\tfor (i = 0; i < GLIBTOP_MAX_"toupper(feature)"; i++)";
  print "\t\tlist = scm_append";
  print "\t\t\t(gh_list (list,";
  print "\t\t\t\t  gh_list (gh_str02scm (gettext";
  print "\t\t\t\t\t\t\t(glibtop_descriptions_"feature" [i]))),";
  print "\t\t\t\t  SCM_UNDEFINED));";
  print "";
  print "\treturn list;";
  print "}";

  print "";
}

/^[^#]/		{
  line = $0;
  split (line, line_fields, /\|/);
  feature = line_fields[2];
  sub(/^@/,"",feature);

  features[feature] = feature;
}

END {
  features["sysdeps"] = "sysdeps";

  for (feature in features) {
    output(feature);
  }

  for (feature in features) {
    feature_name = feature; sub(/_/, "-", feature_name);
    print "SCM_GLOBAL_VCELL (s_names_"feature", \"glibtop-names-"feature_name"\");";
    print "SCM_GLOBAL_VCELL (s_labels_"feature", \"glibtop-labels-"feature_name"\");";
    print "SCM_GLOBAL_VCELL (s_types_"feature", \"glibtop-types-"feature_name"\");";
    print "SCM_GLOBAL_VCELL (s_descriptions_"feature", \"glibtop-descriptions-"feature_name"\");";
  }
  print "";

  print "void";
  print "glibtop_boot_guile_names (void)";
  print "{";
  print "#include \"guile-names.x\"";
  for (feature in features) {
    print "SCM_SETCDR (s_names_"feature", glibtop_guile_names_"feature" ());";
    print "SCM_SETCDR (s_labels_"feature", glibtop_guile_labels_"feature" ());";
    print "SCM_SETCDR (s_types_"feature", glibtop_guile_types_"feature" ());";
    print "SCM_SETCDR (s_descriptions_"feature", glibtop_guile_descriptions_"feature" ());";
  }
  print "}";
}

