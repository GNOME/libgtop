#!/usr/bin/perl

$, = ' ';		# set output field separator
$\ = "\n";		# set output record separator

print '/* lib.c */';
print "/* This is a generated file.  Please modify `lib.pl' */";
print '';

print '#include <config.h>';
print '';
print '#include <glibtop.h>';
print '#include <glibtop/error.h>';
print '#include <glibtop/open.h>';
print '';
print '#include <glibtop/sysdeps.h>';
print '#include <glibtop/union.h>';
print '#include <glib/gi18n-lib.h>';
print '';
print '#include <glibtop/command.h>';

print '';
print '/* Some required fields are missing. */';
print '';

print 'static void';
print '_glibtop_missing_feature (glibtop *server, const char *feature,';
print "\t\t\t  const guint64 present, guint64 *required)";
print '{';
print "\tguint64 old_required = *required;\n";
print "\t/* Return if we have all required fields. */";
print "\tif ((~present & old_required) == 0)";
print "\t\treturn;\n";
print "\tswitch (server->error_method) {";
print "\tcase GLIBTOP_ERROR_METHOD_WARN_ONCE:";
print "\t\t*required &= present;";
print "\tcase GLIBTOP_ERROR_METHOD_WARN:";
print "\t\tglibtop_warn_r (server,";
print "\t\t\t\t_(\"glibtop_get_%s (): Client requested \"";
print "\t\t\t\t  \"field mask %05lx, but only have %05lx.\"),";
print "\t\t\t\t feature, (unsigned long) old_required,";
print "\t\t\t\t (unsigned long) present);";
print "\t\tbreak;";
print "\tcase GLIBTOP_ERROR_METHOD_ABORT:";
print "\t\tglibtop_error_r (server,";
print "\t\t\t\t _(\"glibtop_get_%s (): Client requested \"";
print "\t\t\t\t  \"field mask %05lx, but only have %05lx.\"),";
print "\t\t\t\t feature, (unsigned long) old_required,";
print "\t\t\t\t (unsigned long) present);";
print "\t\tbreak;";
print "\t}";
print '}';

print '';
print '/* Library functions. */';
print '';

$convert{'long'} = 'gint64';
$convert{'ulong'} = 'guint64';
$convert{'pid_t'} = 'pid_t';
$convert{'int'} = 'int';
$convert{'ushort'} = 'unsigned short';
$convert{'unsigned'} = 'unsigned';

while (<>) {
    chop;	# strip record separator

    if (/^[^#]/) {
	&output($_);
    }
}

sub output {
    local($line) = @_;
    @line_fields = split(/\|/, $line, 9999);
    $retval = $line_fields[0];
    $feature = $line_fields[1];
    $param_def = $line_fields[2];

    $orig = $feature;
    $feature =~ s/^@//;
    $space = $feature;
    $space =~ s/./ /g;

    print $retval;
    if ($retval !~ /^void$/) {
	$prefix = 'retval = ';
	$prefix_space = '         ';
    }
    else {
	$prefix = '';
	$prefix_space = '';
    }

    if ($param_def eq 'string') {
	$call_param = ', ' . $line_fields[3];
	$param_buf = '';
	$buf_set = '';
	$param_decl = ",\n            " . $space . '    const char *' .

	  $line_fields[3];
	$send_ptr = "\n\tconst void *send_ptr = " . $line_fields[3] . ';';
	$send_size = "\n\tconst size_t send_size =\n\t\tstrlen (" .

	  $line_fields[3] . ') + 1;';
    }
    else {
	$call_param = '';
	$param_decl = '';
	$send_size = '';
	$send_ptr = '';
	$param_buf = '';
	$buf_set = '';
	$nr_params = (@params = split(/:/, $param_def, 9999));
	if ($nr_params) {
	    $param_buf = "\t\tstruct {\n";
	}
	for ($param = 0; $param < $nr_params; $param++) {
	    $list = $params[$param];
	    $type = $params[$param];
	    $type =~ s/\(.*//;
	    $list =~ s/^.*\(//;
	    $list =~ s/\)$//;
	    $count = (@fields = split(/,/, $list, 9999));

	    if ($count > 0) {
		for ($field = 0; $field < $count; $field++) {
		    $param_buf .= "\t\t\t$convert{$type} buf_$fields[$field];\n";
		    $buf_set .= "\t\tparam_buf.buf_$fields[$field] = $fields[$field];\n";
		}
	    }

	    for ($field = 0; $field < $count; $field++) {
		if ($param_decl eq '') {
		    $param_decl = ",\n            " . $space . '    ';
		}
		else {
		    $param_decl = $param_decl . ', ';
		}
		$param_decl = $param_decl . '' . $convert{$type} . ' ' .

		  $fields[$field];
		$call_param = $call_param . ', ' . $fields[$field];
		if ($send_ptr eq '') {
		    $send_ptr = "\t\tconst void *send_ptr = &param_buf;";
		}
	    }
	}
	if ($nr_params) {
	    $param_buf .= "\t\t} param_buf;\n";
	    $send_size  = "\t\tconst size_t send_size = sizeof param_buf;";
	}
	else {
	    $send_size = "\t\tconst size_t send_size = 0;";
	}
	if ($send_ptr eq '') {
	    $send_ptr = "\t\tconst void *send_ptr = NULL;";
	}
    }

    print 'glibtop_get_' . $feature . '_l (glibtop *server, glibtop_' .

      $feature . ' *buf' . $param_decl . ')';

    print "{";

    if ($retval !~ /^void$/) {
	print "\t" . $retval . ' retval = (' . $retval . ') 0;';
    }

    print "\tglibtop_init_r (&server, (1 << GLIBTOP_SYSDEPS_" .

      uc($feature) . '), 0);';

    print '';
    print "\t/* If neccessary, we ask the server for the requested";
    print "\t * feature. If not, we call the sysdeps function. */";
    print '';

    print "\tif ((server->flags & _GLIBTOP_INIT_STATE_SERVER) &&";
    print "\t    (server->features & (1 << GLIBTOP_SYSDEPS_" .

      uc($feature) . ')))';
    print "\t{";

    if ($param_buf) {
        print $param_buf;
        print $buf_set;
    }

    print $send_ptr;
    print $send_size;
    print '';

    print "\t\t" . $prefix . 'glibtop_call_l (server, GLIBTOP_CMND_' .

      uc($feature) . ',';
    print "\t\t\t\t" . $prefix_space . 'send_size, send_ptr,';
    print "\t\t\t\t" . $prefix_space . 'sizeof (glibtop_' . $feature .

      '), buf);';

    print "\t} else {";

    if ($orig !~ /^@/) {
	print '#if (!GLIBTOP_SUID_' . uc($feature) . ')';
    }
    print "\t\t" . $prefix . 'glibtop_get_' . $feature . '_s (server, buf' .

      $call_param . ');';

    if ($orig !~ /^@/) {
	print '#else';
	print "\t\terrno = ENOSYS;";
	print "\t\tglibtop_error_io_r (server, \"glibtop_get_" . $feature .

	  "\");";
	print '#endif';
    }

    print "\t}";

    print '';
    print "\t/* Make sure that all required fields are present. */";
    print '';

    print "\tif (buf->flags & server->required." . $feature . ')';
    print "\t\t_glibtop_missing_feature (server, \"" . $feature .

      "\", buf->flags,";
    print "\t\t\t\t\t  &server->required." . $feature . ');';

    if ($retval !~ /^void$/) {
	print "\n\t/* Now we can return. */";
	print "\n\treturn retval;";
    }

    print '}';
    print '';
}
