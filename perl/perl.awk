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
  print "";
  print "MODULE = Libgtop\t\tPACKAGE = Libgtop";
  print "";

  convert["long"]   = "newSViv";
  convert["ulong"]  = "newSViv";
  convert["double"] = "newSVnv";
}

/^(\w+)/ {
  feature = $1;

  print "void";
  if (feature ~ /^proc_/) {
    print feature"(pid)";
    print "\tunsigned\tpid;";
  } else {
    print feature"()";
  }

  print "PREINIT:";
  print "\tglibtop_"feature" "feature";";

  if (feature ~ /^proclist/) {
    print "\tunsigned i, *ptr;";
    print "PPCODE:";
    print "\tptr = glibtop_get_proclist (&proclist);";
    print "";
    print "\tif (ptr) {";
    print "\t\tfor (i = 0; i < proclist.number; i++)";
    print "\t\t\tXPUSHs (sv_2mortal (newSViv (ptr [i])));";
    print "\t}";
    print "";
    print "\tglibtop_free (ptr);";
  } else {
    print "PPCODE:";
    if (feature ~ /^proc_/) {
      print "\tglibtop_get_"feature" (&"feature", pid);";
    } else {
      print "\tglibtop_get_"feature" (&"feature");";
    }
    print "";
    
    nr_elements = split ($2, elements, /:/);
    for (element = 1; element <= nr_elements; element++) {
      list = elements[element];
      type = elements[element];
      sub(/\(.*/, "", type);
      sub(/^\w+\(/, "", list); sub(/\)$/, "", list);
      count = split (list, fields, /,/);
      for (field = 1; field <= count; field++) {
	if (type ~ /^str$/) {
	  print "\tXPUSHs (sv_2mortal (newSVpv ("$1"."fields[field]", 0)));";
	} else {
	  if (type ~ /^char$/) {
	    print "\tXPUSHs (sv_2mortal (newSVpv (&"$1"."fields[field]", 1)));";
	  } else {
	    print "\tXPUSHs (sv_2mortal ("convert[type]" ("$1"."fields[field]")));";
	  }
	}
      }
    }
  }
    
  print "";
}

