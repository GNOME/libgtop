#!/usr/bin/perl

require 'c_types.pl';

$[ = 1;			# set array base to 1
$, = ' ';		# set output field separator
$\ = "\n";		# set output record separator

$always_use_temp_storage = 1;

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

print '/* lib.c */';
print "/* This is a generated file.  Please modify `lib.pl' */";
print '';

print '#include <glibtop.h>';
print '#include <glibtop/open.h>';
print '#include <glibtop/xmalloc.h>';
print '';
print '#include <glibtop/sysdeps.h>';
print '#include <glibtop/union.h>';
print '';
print '#include <glibtop/command.h>';

print '';
print '/* Some required fields are missing. */';
print '';

print 'static void';
print '_glibtop_missing_feature (glibtop *server, const char *feature,';
print "\t\t\t  const u_int64_t present, u_int64_t *required)";
print '{';
print "\tu_int64_t old_required = *required;\n";
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

while (<>) {
    chop;	# strip record separator

    if (/^[^#]/) {
	&output($_);
    }
}

sub output {
    local($line) = @_;
    @line_fields = split(/\|/, $line, 9999);
    $retval = $line_fields[1];
    $feature = $line_fields[2];
    $param_def = $line_fields[4];

    $orig = $feature;
    $feature =~ s/^@//;
    $space = $feature;
    $space =~ s/./ /g;

    if ($retval eq 'retval') {
      $retval_param = '&retval';
      $call_prefix = '';
      $call_prefix_space = '';
      $prefix = 'retval = ';
      $prefix_space = '         ';
      $retval = 'int';
    }
    elsif ($retval !~ /^void$/) {
      $retval_param = 'NULL';
      $prefix = 'retval = ';
      $prefix_space = '         ';
      $call_prefix = 'retval = ';
      $call_prefix_space = '         ';
    }
    else {
      $retval_param = 'NULL';
      $call_prefix = '';
      $call_prefix_space = '';
      $prefix = '';
      $prefix_space = '';
    }

    $check_server_code = "\n";
    $check_server_code .=
      "\t/* If neccessary, we ask the server for the requested\n" .
	"\t * feature. If not, we call the sysdeps function. */\n\n" .
	  "\tif ((server->flags & _GLIBTOP_INIT_STATE_SERVER) &&\n" .
	    "\t    (server->features & (1 << GLIBTOP_SYSDEPS_" .
	      &toupper($feature) . ")))\n" .
		"\t\tgoto call_server;\n\telse\n" .
		  "\t\tgoto call_sysdeps;\n\n";

    $need_temp_storage = $always_use_temp_storage;
    $size_code = "\t_LIBGTOP_SEND_len = 0;\n";
    $marshal_code = "\t_LIBGTOP_SEND_offset = 0;\n";
    $marshal_code .= "\tmemset (_LIBGTOP_SEND_buf, 0, _LIBGTOP_SEND_len);\n";
    $first_param_name = '';

    $call_param = '';
    $param_decl = '';
    $need_temp_len = 0;
    $nr_params = (@params = split(/:/, $param_def, 9999));
    for ($param = 1; $param <= $nr_params; $param++) {
      $list = $params[$param];
      $type = $params[$param];
      $type =~ s/\(.*//;
      $list =~ s/^.*\(//;
      $list =~ s/\)$//;
      $count = (@fields = split(/,/, $list, 9999));
      for ($field = 1; $field <= $count; $field++) {
	my $c_type = $typeinfo->{$type}->[1];

	if ($first_param_name eq '') {
	  $first_param_name = $fields[$field];
	}

	if ($typeinfo->{$type}->[2]) {
	  $need_temp_storage = 1;
	}

	if ($param_decl eq '') {
	  $param_decl = ",\n            " . $space . '    ';
	}
	else {
	  $param_decl = $param_decl . ', ';
	}
	$param_decl = $param_decl . $c_type . ' ' . $fields[$field];
	$call_param = $call_param . ', ' . $fields[$field];

	$size_code .= "\t_LIBGTOP_SEND_len += ";
	if (defined $sizeof_funcs->{$type}) {
	  $size_code .= $sizeof_funcs->{$type}->($fields[$field]);
	} else {
	  $size_code .= sprintf ("sizeof (%s)", $c_type);
	}
	$size_code .= ";\n";

	$marshal_code .= "\t_LIBGTOP_SEND_ptr = ".
	  "(char *) _LIBGTOP_SEND_buf + _LIBGTOP_SEND_offset;\n";

	if (defined $marshal_funcs->{$type}) {
	  $marshal_code .= $marshal_funcs->{$type}->($c_type, $fields[$field], "\t");
	} else {
	  $marshal_code .= sprintf ("\tmemcpy (_LIBGTOP_SEND_ptr, %s, %s);\n",
				    '&'.$fields[$field], "sizeof ($c_type)");
	}

	$marshal_code .= "\t_LIBGTOP_SEND_offset += ";
	if (defined $sizeof_funcs->{$type}) {
	  $marshal_code .= $sizeof_funcs->{$type}->($fields[$field]);
	} else {
	  $marshal_code .= sprintf ("sizeof (%s)", $c_type);
	}
	$marshal_code .= ";\n";
      }
    }

    $local_var_code = "";
    $local_var_code .= "\tunsigned _LIBGTOP_SEND_offset, _LIBGTOP_SEND_len;\n";
    if ($need_temp_len) {
      $local_var_code .= "\tunsigned _LIBGTOP_SEND_temp_len;\n";
    }
    $local_var_code .= "\tvoid *_LIBGTOP_SEND_buf;\n";
    $local_var_code .= "\tchar *_LIBGTOP_SEND_ptr;\n";
    if ($retval !~ /^void$/) {
      $local_var_code .= sprintf ("\t%s retval = (%s) 0;\n",
				  $retval, $retval);
    }

    $sysdeps_code = "call_sysdeps:\n";
    if ($orig !~ /^@/) {
      $sysdeps_code .= sprintf ("#if (!GLIBTOP_SUID_%s)\n", &toupper($feature));
    }

    if ($line_fields[3] eq '') {
      $sysdeps_code .= sprintf ("\t%sglibtop_get_%s_s (server%s);\n",
				$prefix, $feature, $call_param);
    } else {
      $sysdeps_code .= sprintf ("\t%sglibtop_get_%s_s (server, buf%s);\n",
				$prefix, $feature, $call_param);
    }
    $sysdeps_code .= "\tgoto check_missing;\n";

    if ($orig !~ /^@/) {
      $sysdeps_code .= "#else\n\terrno = ENOSYS;\n";
      $sysdeps_code .= sprintf ("\tglibtop_error_io_r (server, \"%s\");\n",
				"glibtop_get_" . $feature);
      $sysdeps_code .= "\tgoto do_return;\n";
      $sysdeps_code .= "#endif\n";
    }

    $total_code = sprintf ("%s%s%s\n", $init_code, $check_server_code,
			   $sysdeps_code);

    $send_buf_code = "\t_LIBGTOP_SEND_buf = ";
    if ($need_temp_storage) {
      $send_buf_code .= "glibtop_malloc_r (server, _LIBGTOP_SEND_len+1)";
    } else {
      $send_buf_code .= '(void *) &'.$first_param_name;
    }
    $send_buf_code .= ";\n";

    $call_code = '';
    $call_code .= sprintf ("\t%sglibtop_call_l (server, GLIBTOP_CMND_%s,\n",
			   $call_prefix, &toupper($feature));
    $call_code .= sprintf ("\t\t\t%s%s, %s,\n", $call_prefix_space,
			   "_LIBGTOP_SEND_len", "_LIBGTOP_SEND_ptr");
    if ($line_fields[3] eq '') {
      $call_code .= sprintf ("\t\t\t%s0, NULL,\n", $call_prefix_space);
    } else {
      $call_code .= sprintf ("\t\t\t%ssizeof (glibtop_%s), buf,\n",
			     $call_prefix_space, $feature);
    }
    $call_code .= sprintf ("\t\t\t%s%s);\n", $call_prefix_space, $retval_param);

    if ($need_temp_storage) {
      $send_buf_free_code = "\tglibtop_free_r (server, _LIBGTOP_SEND_buf);\n";
    } else {
      $send_buf_free_code = "";
    }

    $total_code .= sprintf ("call_server:\n%s\n%s\n%s\n%s\n%s\n",
			    $size_code, $send_buf_code, $marshal_code,
			    $call_code, $send_buf_free_code);

    $total_code .= "\tgoto check_missing;\n\n";

    $check_code = "check_missing:\n";
    $check_code .= "\t/* Make sure that all required fields are present. */\n";
    if (!($line_fields[3] eq '')) {
      $check_code .= "\tif (buf->flags & server->required." . $feature . ")\n";
      $check_code .= "\t\t_glibtop_missing_feature (server, \"" . $feature .
	"\", buf->flags,\n\t\t\t\t\t  &server->required." . $feature . ");\n";
    }

    $total_code .= $check_code."\tgoto do_return;\n\n";

    $return_code = "do_return:\n";
    if ($retval !~ /^void$/) {
      $return_code .= "\treturn retval;\n";
    } else {
      $return_code .= "\treturn;\n";
    }

    $total_code .= $return_code;

    $func_decl = $retval."\n";
    if ($line_fields[3] eq '') {
      $func_decl .= sprintf ("glibtop_get_%s_l (glibtop *server%s)\n",
			     $feature, $param_decl);
    } else {
      $func_decl .= sprintf ("glibtop_get_%s_l (glibtop *server, %s *buf%s)\n",
			     $feature, 'glibtop_'.$feature, $param_decl);
    }

    $init_code = sprintf ("\tglibtop_init_r (&server, (1 << %s), 0);\n",
			  "GLIBTOP_SYSDEPS_".&toupper($feature));

    $total_code = sprintf ("%s{\n%s\n%s\n%s}\n", $func_decl,
			   $local_var_code, $total_code);

    print $total_code;
}
