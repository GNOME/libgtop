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

print '/* guile.c */';
print "/* This is a generated file.  Please modify `guile.pl' */";
print '';

print '#include <glibtop.h>';
print '#include <glibtop/xmalloc.h>';
print '#include <glibtop/sysdeps.h>';
print '#include <glibtop/union.h>';
print '';
print '#include <guile/gh.h>';

print '';

$convert{'long'} = 'gh_long2scm  ';
$convert{'ulong'} = 'gh_ulong2scm ';
$convert{'double'} = 'gh_double2scm';
$convert{'str'} = 'gh_str02scm  ';
$convert{'char'} = 'gh_char2scm  ';

$backconv{'int'} = 'gh_scm2long';
$backconv{'pid_t'} = 'gh_scm2ulong';
$backconv{'long'} = 'gh_scm2long';
$backconv{'ulong'} = 'gh_scm2ulong';
$backconv{'unsigned'} = 'gh_scm2ulong';

$feature_count = 0;

while (<>) {
    chop;	# strip record separator

    if (/^[^#]/) {
	&make_output($_);
    }
}

$sep = '';
$sysdeps = 'void|sysdeps|ulong(';
for ($nr = 0; $nr < $feature_count; $nr++) {
    $sysdeps = $sysdeps . '' . $sep . '' . $feature_field{$nr};
    $sep = ',';
}
$sysdeps = $sysdeps . ')';
&make_output($sysdeps);
print 'void';
print 'glibtop_boot_guile (void)';
print '{';
print "#include \"guile.x\"";
print '}';

sub make_output {
    local($line) = @_;
    @line_fields = split(/\|/, $line, 9999);
    $retval = $line_fields[1];
    $element_def = $line_fields[3];
    $feature = $line_fields[2];
    $param_def = $line_fields[4];

    $feature =~ s/^@//;
    $features{$feature} = $feature;

    $feature_field{$feature_count} = $feature;
    $feature_count = $feature_count + 1;

    $total_nr_params = 0;

    if ($param_def eq 'string') {
	$call_param = ', gh_scm2newstr( ' . $line_fields[5] . ', NULL)';
	$param_decl = 'SCM ' . $line_fields[5];
	$total_nr_params = 1;
    }
    else {
	$call_param = '';
	$param_decl = '';
	$nr_params = (@params = split(/:/, $param_def, 9999));
	for ($param = 1; $param <= $nr_params; $param++) {
	    $list = $params[$param];
	    $type = $params[$param];
	    $type =~ s/\(.*//;
	    $list =~ s/^.*\(//;
	    $list =~ s/\)$//;
	    $count = (@fields = split(/,/, $list, 9999));
	    $total_nr_params = $total_nr_params + $count;
	    for ($field = 1; $field <= $count; $field++) {
		if ($param_decl ne '') {
		    $param_decl = $param_decl . ', ';
		}
		$param_decl = $param_decl . 'SCM ' . $fields[$field];
		$call_param = $call_param . ', ' . $backconv{$type} . ' (' .

		  $fields[$field] . ')';
	    }
	}
	if ($param_decl eq '') {
	    $param_decl = 'void';
	}
    }

    $nr_params_field{$feature} = $total_nr_params;

    $feature_name = $feature;
    $feature_name =~ s/_/-/;
    $output = 'SCM_PROC (s_' . $feature . ", \"glibtop-get-" . $feature_name .

      "\",";
    $output = $output . ' ' . $nr_params_field{$feature} . ', 0, 0, ';
    $output = $output . 'glibtop_guile_get_' . $feature . ");\n\n";

    $output = $output . "static SCM\nglibtop_guile_get_" . $feature . ' (' .

      $param_decl . ")\n{\n";

    $output = $output . "\tglibtop_" . $feature . ' ' . $feature . ";\n";
    if ($retval ne 'void') {
	$output = $output . "\t" . $retval . " retval;\n";
    }
    if ($feature =~ /^(proc(list|_map|_args))|mountlist$/) {
	$output = $output . "\tunsigned i;\n";
    }
    if ($feature =~ /^proc_args$/) {
	$output = $output . "\tSCM list, scm_args, args_list;\n";
	$output = $output . "\tchar *start;\n\n";
    }
    else {
	$output = $output . "\tSCM list;\n\n";
    }
    if ($retval ne 'void') {
	$prefix = 'retval = ';
    }
    else {
	$prefix = '';
    }
    $output = $output . "\t" . $prefix . 'glibtop_get_' . $feature . ' (&' .

      $feature . '' . $call_param . ");\n\n";

    $output = $output . "\tlist = gh_list (gh_ulong2scm  (" . $feature .

      ".flags),\n\t\t\t";

    $nr_elements = (@elements = split(/:/, $element_def, 9999));
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
		$output = $output . "gh_list\n\t\t\t(";
		for ($nr = 0; $nr < $number; $nr++) {
		    $output = $output . '' . $convert{$type} . ' (' . $feature

		      . '.' . $fields[$field] . ' [' . $nr . "]),\n\t\t\t ";
		}
		$output = $output . "SCM_UNDEFINED),\n\t\t\t";
	    }
	    else {
		$output = $output . '' . $convert{$type} . ' (' . $feature .

		  '.' . $fields[$field] . "),\n\t\t\t";
	    }
	}
    }
    $output = $output . "SCM_UNDEFINED);\n";

    print $output;

    if ($feature =~ /^proclist$/) {
	print "\tif (retval == NULL)";
	print "\t\treturn list;";
	print '';
	print "\tfor (i = 0; i < proclist.number; i++)";
	print "\t\tlist = scm_append";
	print "\t\t\t(gh_list (list,";
	print

	  "\t\t\t\t  gh_list (gh_ulong2scm ((unsigned long) retval [i])),";
	print "\t\t\t\t  SCM_UNDEFINED));";
	print '';
	print "\tglibtop_free (retval);\n";
    }

    if ($feature =~ /^proc_map$/) {
	print "\tif (retval == NULL)";
	print "\t\treturn list;";
	print '';
	print "\tfor (i = 0; i < proc_map.number; i++) {";
	print "\t\tglibtop_map_entry *entry = &(retval [i]);";
	print "\t\tSCM scm_entry = gh_list";
	print "\t\t\t(gh_ulong2scm ((unsigned long) entry->flags),";
	print "\t\t\t gh_ulong2scm ((unsigned long) entry->start),";
	print "\t\t\t gh_ulong2scm ((unsigned long) entry->end),";
	print "\t\t\t gh_ulong2scm ((unsigned long) entry->offset),";
	print "\t\t\t gh_ulong2scm ((unsigned long) entry->perm),";
	print "\t\t\t gh_ulong2scm ((unsigned long) entry->inode),";
	print "\t\t\t gh_ulong2scm ((unsigned long) entry->device),";
	print "\t\t\t gh_str02scm (entry->filename), SCM_UNDEFINED);";
	print "\t\tSCM entry_list = gh_list (scm_entry, SCM_UNDEFINED);\n";

	print

	  "\t\tlist = scm_append (gh_list (list, entry_list, SCM_UNDEFINED));";
	print "\t};\n";
	print "\tglibtop_free (retval);\n";
    }

    if ($feature =~ /^proc_args$/) {
	print "\tif (retval == NULL)";
	print "\t\treturn list;";
	print '';
	print "\tstart = retval;";
	print "\tscm_args = gh_list (SCM_UNDEFINED);\n";
	print "\tfor (i = 0; i <= proc_args.size; i++) {";
	print "\t\tSCM arg_list;\n";
	print "\t\tif (retval [i]) continue;\n";
	print "\t\targ_list = gh_list (gh_str02scm (start), SCM_UNDEFINED);";
	print "\t\tscm_args = scm_append";
	print "\t\t\t(gh_list (scm_args, arg_list, SCM_UNDEFINED));\n;";
	print "\t\tstart = &(retval [i+1]);";
	print "\t};\n";
	print "\targs_list = gh_list (scm_args, SCM_UNDEFINED);";
	print

	  "\tlist = scm_append (gh_list (list, args_list, SCM_UNDEFINED));\n";
	print "\tglibtop_free (retval);\n";
    }

    if ($feature =~ /^mountlist$/) {
	print "\tif (retval == NULL)";
	print "\t\treturn list;";
	print '';
	print "\tfor (i = 0; i < mountlist.number; i++) {";
	print "\t\tglibtop_mountentry *entry = &(retval [i]);";
	print "\t\tSCM scm_entry = gh_list";
	print "\t\t\t(gh_ulong2scm ((unsigned long) entry->dev),";
	print "\t\t\t gh_str02scm (entry->devname),";
	print "\t\t\t gh_str02scm (entry->mountdir),";
	print "\t\t\t gh_str02scm (entry->type), SCM_UNDEFINED);";
	print "\t\tSCM entry_list = gh_list (scm_entry, SCM_UNDEFINED);\n";

	print

	  "\t\tlist = scm_append (gh_list (list, entry_list, SCM_UNDEFINED));";
	print "\t};\n";
	print "\tglibtop_free (retval);\n";
    }

    print "\treturn list;";
    print '}';
    print '';
}
