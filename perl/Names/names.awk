BEGIN {
  print "/* Names.xs */";
  print "/* This is a generated file.  Please modify `names.awk' */";
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
  print "MODULE = Libgtop::Names\t\tPACKAGE = Libgtop::Names";
  print "";
  print "PROTOTYPES: ENABLE";
  print "";
}

function output(line) {
  split (line, line_fields, /\|/);
  retval = line_fields[1];
  feature = line_fields[2];
  element_def = line_fields[3];
  param_def = line_fields[4];

  orig = feature; sub(/^@/,"",feature);
  space = feature; gsub(/./," ",space);

  features[feature] = feature;
}

/^[^#]/		{ output($0) }

END {
  print "BOOT:";
  print "{";
  print "\tHV *names = perl_get_hv (\"Libgtop::Names::names\", TRUE);";
  print "\tHV *labels = perl_get_hv (\"Libgtop::Names::labels\", TRUE);";
  print "\tHV *types = perl_get_hv (\"Libgtop::Names::types\", TRUE);";
  print "\tHV *descriptions = perl_get_hv (\"Libgtop::Names::descriptions\", TRUE);";
  print "\tAV *array;";
  print "\tSV *ref;";
  print "\tint i;";
  
  for (feature in features) {
    print "\tarray = newAV ();";
    print "\tref = newRV_inc ((SV*)array);";
    print "\tfor (i = 0; i < GLIBTOP_MAX_"toupper(feature)"; i++)";
    print "\t{";
    print "\t\tav_push (array, newSVpv ((char*)glibtop_names_"feature" [i], 0));";
    print "\t}";
    print "\thv_store (names, \""feature"\", "length(feature)+1", ref, 0);";
    print "\tarray = newAV ();";
    print "\tref = newRV_inc ((SV*)array);";
    print "\tfor (i = 0; i < GLIBTOP_MAX_"toupper(feature)"; i++)";
    print "\t{";
    print "\t\tav_push (array, newSVpv ((char*)_(glibtop_labels_"feature" [i]), 0));";
    print "\t}";
    print "\thv_store (labels, \""feature"\", "length(feature)+1", ref, 0);";
    print "\tarray = newAV ();";
    print "\tref = newRV_inc ((SV*)array);";
    print "\tfor (i = 0; i < GLIBTOP_MAX_"toupper(feature)"; i++)";
    print "\t{";
    print "\t\tav_push (array, newSViv (glibtop_types_"feature" [i]));";
    print "\t}";
    print "\thv_store (types, \""feature"\", "length(feature)+1", ref, 0);";

    print "\tarray = newAV ();";
    print "\tref = newRV_inc ((SV*)array);";
    print "\tfor (i = 0; i < GLIBTOP_MAX_"toupper(feature)"; i++)";
    print "\t{";
    print "\t\tav_push (array, newSVpv ((char*)_(glibtop_descriptions_"feature" [i]), 0));";
    print "\t}";
    print "\thv_store (descriptions, \""feature"\", "length(feature)+1", ref, 0);";
  }

  print "}";
  print "";
}
