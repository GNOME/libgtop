#!/usr/bin/perl

$[ = 1;			# set array base to 1
$, = ' ';		# set output field separator
$\ = "\n";		# set output record separator

sub toupper {
    local($_) = @_;
    tr/a-z/A-Z/;
    return $_;
}

sub tolower {
    local($_) = @_;
    tr/A-Z/a-z/;
    return $_;
}

print '/* guile_names.c */';
print "/* This is a generated file.  Please modify `guile-names.pl' */";
print '';

print '#include <glibtop.h>';
print '#include <glibtop/sysdeps.h>';
print '#include <glibtop/union.h>';
print '';
print '#include <guile/gh.h>';

print '';

while (<>) {
    chop;	# strip record separator

    if (/^[^#]/) {
	$line = $_;
	@line_fields = split(/\|/, $line, 9999);
	$feature = $line_fields[2];
	$element_def = $line_fields[3];
	$feature =~ s/^@//;

	$features{$feature} = $feature;
	$element_defs{$feature} = $element_def;
    }
}

$features{'sysdeps'} = 'sysdeps';

foreach $feature (keys %features) {
    &output($feature);
}

foreach $feature (keys %features) {
    $feature_name = $feature;
    $feature_name =~ s/_/-/;
    print 'SCM_GLOBAL_VCELL (s_names_' . $feature . ", \"glibtop-names-" .

      $feature_name . "\");";
    print 'SCM_GLOBAL_VCELL (s_labels_' . $feature . ", \"glibtop-labels-" .

      $feature_name . "\");";
    print 'SCM_GLOBAL_VCELL (s_types_' . $feature . ", \"glibtop-types-" .

      $feature_name . "\");";
    print 'SCM_GLOBAL_VCELL (s_descriptions_' . $feature .

      ", \"glibtop-descriptions-" . $feature_name . "\");";
}
print '';

print 'void';
print 'glibtop_boot_guile_names (void)';
print '{';
print "#include \"guile-names.x\"";
foreach $feature (keys %features) {
    print 'SCM_SETCDR (s_names_' . $feature . ', glibtop_guile_names_' .

      $feature . ' ());';
    print 'SCM_SETCDR (s_labels_' . $feature . ', glibtop_guile_labels_' .

      $feature . ' ());';
    print 'SCM_SETCDR (s_types_' . $feature . ', glibtop_guile_types_' .

      $feature . ' ());';
    print 'SCM_SETCDR (s_descriptions_' . $feature .

      ', glibtop_guile_descriptions_' . $feature . ' ());';
}
print '}';

sub output {
    local($feature) = @_;
    print 'static SCM';
    print 'glibtop_guile_names_' . $feature . ' (void)';
    print '{';
    print "\tint i;";
    print "\tSCM list;";
    print '';
    print "\tlist = gh_list (SCM_UNDEFINED);";
    print '';
    print "\tfor (i = 0; i < GLIBTOP_MAX_" . &toupper($feature) . '; i++)';
    print "\t\tlist = scm_append";
    print "\t\t\t(gh_list (list,";
    print "\t\t\t\t  gh_list (gh_str02scm ((char *) glibtop_names_" . $feature

      . ' [i])),';
    print "\t\t\t\t  SCM_UNDEFINED));";
    print '';
    print "\treturn list;";
    print '}';

    print '';

    print 'static SCM';
    print 'glibtop_guile_types_' . $feature . ' (void)';
    print '{';
    print "\tSCM list;";
    print '';

    $out = "\tlist = gh_list (";

    $nr_elements = (@elements = split(/:/, $element_defs{$feature}, 9999));
    for ($element = 1; $element <= $nr_elements; $element++) {
	$list = $elements[$element];
	$type = $elements[$element];
	$type =~ s/\(.*//;
	$list =~ s/^.*\(//;
	$list =~ s/\)$//;
	$count = (@fields = split(/,/, $list, 9999));
	for ($field = 1; $field <= $count; $field++) {
	    if ($fields[$field] =~ /^(\w+)\[([0-9]+)\]$/) {
		@field_parts = split(/\[/, $fields[$field], 9999);
		$fields[$field] = $field_parts[1];
		$field_parts[2] =~ s/\]//;
		$number = $field_parts[2];
		$out = $out . "gh_cons\n\t\t\t";
		$out = $out . '(gh_ulong2scm (glibtop_types_' . $feature .

		  ' [' . ($field - 1) . "]),\n\t\t\t";
		$out = $out . ' gh_ulong2scm (' . $number . ")),\n\t\t\t";
	    }
	    else {
		$out = $out . 'gh_ulong2scm (glibtop_types_' . $feature . ' ['

		  . ($field - 1) . "]),\n\t\t\t";
	    }
	}
    }

    print $out . 'SCM_UNDEFINED);';
    print '';
    print "\treturn list;";
    print '}';

    print '';

    print 'static SCM';
    print 'glibtop_guile_labels_' . $feature . ' (void)';
    print '{';
    print "\tint i;";
    print "\tSCM list;";
    print '';
    print "\tlist = gh_list (SCM_UNDEFINED);";
    print '';
    print "\tfor (i = 0; i < GLIBTOP_MAX_" . &toupper($feature) . '; i++)';
    print "\t\tlist = scm_append";
    print "\t\t\t(gh_list (list,";
    print "\t\t\t\t  gh_list (gh_str02scm (_(glibtop_labels_" . $feature .

      ' [i]))),';
    print "\t\t\t\t  SCM_UNDEFINED));";
    print '';
    print "\treturn list;";
    print '}';

    print '';

    print 'static SCM';
    print 'glibtop_guile_descriptions_' . $feature . ' (void)';
    print '{';
    print "\tint i;";
    print "\tSCM list;";
    print '';
    print "\tlist = gh_list (SCM_UNDEFINED);";
    print '';
    print "\tfor (i = 0; i < GLIBTOP_MAX_" . &toupper($feature) . '; i++)';
    print "\t\tlist = scm_append";
    print "\t\t\t(gh_list (list,";
    print "\t\t\t\t  gh_list (gh_str02scm (_(glibtop_descriptions_" . $feature

      . ' [i]))),';
    print "\t\t\t\t  SCM_UNDEFINED));";
    print '';
    print "\treturn list;";
    print '}';

    print '';
}
