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
print '#include <glibtop/glibtop-client.h>';
print '';
print '#include <glibtop/call-vector.h>';
print '#include <glibtop-client-private.h>';

print '';
print '/* Some required fields are missing. */';
print '';

print '#if 0';
print '';

print 'static void';
print '_glibtop_missing_feature (glibtop_client *client, const char *feature,';
print "\t\t\t  const u_int64_t present, u_int64_t *required)";
print '{';
print "\tu_int64_t old_required = *required;\n";
print "\t/* Return if we have all required fields. */";
print "\tif ((~present & old_required) == 0)";
print "\t\treturn;\n";
print "\tswitch (client->_param.error_method) {";
print "\tcase GLIBTOP_ERROR_METHOD_WARN_ONCE:";
print "\t\t*required &= present;";
print "\tcase GLIBTOP_ERROR_METHOD_WARN:";
print "\t\tglibtop_warn_r (client,";
print "\t\t\t\t\"glibtop_get_%s (): Client requested \"";
print "\t\t\t\t\"field mask %05lx, but only have %05lx.\",";
print "\t\t\t\t feature, (unsigned long) old_required,";
print "\t\t\t\t (unsigned long) present);";
print "\t\tbreak;";
print "\tcase GLIBTOP_ERROR_METHOD_ABORT:";
print "\t\tglibtop_error_r (client,";
print "\t\t\t\t\"glibtop_get_%s (): Client requested \"";
print "\t\t\t\t\"field mask %05lx, but only have %05lx.\",";
print "\t\t\t\t feature, (unsigned long) old_required,";
print "\t\t\t\t (unsigned long) present);";
print "\t\tbreak;";
print "\t}";
print '}';

print '';
print '#endif';

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

    if ($retval =~ /^(array|pointer)\((.*)\)$/) {
      $retval = ($2 eq 'string') ? 'char **' : "$2 *";
    }

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
      }
    }

    $local_var_code = "\tGSList *list;\n\tint done = 0;\n\tGError *error = NULL;\n\n";
    if ($retval !~ /^void$/) {
      $local_var_code .= sprintf ("\t%s retval = (%s) 0;\n",
				  $retval, $retval);
    }

    $sysdeps_code = sprintf
      ("\tif (client->_priv->backend_list == NULL) {\n\t\tg_set_error (&error, GLIBTOP_ERROR, GLIBTOP_ERROR_NO_BACKEND_OPENED, G_STRLOC);\n");
    if ($line_fields[1] eq 'retval') {
      $sysdeps_code .= sprintf
	("\t\treturn -GLIBTOP_ERROR_NO_BACKEND_OPENED;\n");
    } else {
      $sysdeps_code .= sprintf
	("\t\tgoto do_return;\n");
    }
    $sysdeps_code .= sprintf
      ("\t}\n\n");

    $sysdeps_code .= sprintf
      ("\tfor (list = client->_priv->backend_list; list; list = list->next) {\n\t\tglibtop_backend *backend = list->data;\n\t\tglibtop_call_vector *call_vector;\n\n\t\tcall_vector = glibtop_backend_get_call_vector (backend);\n\n\t\tif (call_vector && call_vector->%s) {\n\t\t\tglibtop_server *server = glibtop_backend_get_server (backend);\n\n", $feature);

    if ($line_fields[3] eq '') {
      $sysdeps_code .= sprintf
	("\t\t\tretval = call_vector->%s (server, backend%s);\n",
	 $feature, $call_param);
    } elsif ($line_fields[3] eq 'array') {
      $sysdeps_code .= sprintf
	("\t\t\tretval = call_vector->%s (server, backend, array%s);\n",
	 $feature, $call_param);
    } elsif ($line_fields[3] =~ /^array/) {
      $sysdeps_code .= sprintf
	("\t\t\tretval = call_vector->%s (server, backend, array, buf%s);\n",
	 $feature, $call_param);
    } else {
      $sysdeps_code .= sprintf
	("\t\t\tretval = call_vector->%s (server, backend, buf%s);\n",
	 $feature, $call_param);
    }

    $sysdeps_code .= sprintf
      ("\t\t\tdone = 1;\n\t\t\tglibtop_server_unref (server);\n\t\t\t\tbreak;\n\t\t}\n\t}\n");

    $sysdeps_code .= sprintf
      ("\n\tif (!done) {\n\t\tg_set_error (&error, GLIBTOP_ERROR, GLIBTOP_ERROR_NOT_IMPLEMENTED, G_STRLOC);\n");
    if ($line_fields[1] eq 'retval') {
      $sysdeps_code .= sprintf
	("\t\treturn -GLIBTOP_ERROR_NOT_IMPLEMENTED;\n");
    } else {
      $sysdeps_code .= sprintf
	("\t\tgoto do_return;\n");
    }
    $sysdeps_code .= sprintf
      ("\t}\n\n");

    if ($line_fields[1] eq 'retval') {
      $sysdeps_code .= "\tif (retval < 0) {\n";
      $sysdeps_code .= "\t\tg_set_error (&error, GLIBTOP_ERROR, -retval, G_STRLOC);\n";
      $sysdeps_code .= "\t\tgoto do_return;\n";
      $sysdeps_code .= "\t}\n\n";
    }
      
    $sysdeps_code .= "\tgoto check_missing;\n";

#    $init_code = sprintf ("\tglibtop_init_r (&client, (1 << %s), 0);\n\n",
#			  "GLIBTOP_SYSDEPS_".&toupper($feature));
    $init_code = '';

    $total_code = sprintf ("%s%s\n", $init_code, $sysdeps_code);

    $check_code = "check_missing:\n";
    $check_code .= "\t/* Make sure that all required fields are present. */\n";
    if (!(($line_fields[3] eq '') or ($line_fields[3] eq 'array'))) {
#      $check_code .= "\tif (buf->flags & client->info->required." . $feature . ")\n";
#      $check_code .= "\t\t_glibtop_missing_feature (client, \"" . $feature .
	"\", buf->flags,\n\t\t\t\t\t  &client->info->required." . $feature . ");\n";
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
      $func_decl .= sprintf ("glibtop_get_%s_l (glibtop_client *client%s)\n",
			     $feature, $param_decl);
    } elsif ($line_fields[3] eq 'array') {
      $func_decl .= sprintf ("glibtop_get_%s_l (glibtop_client *client, glibtop_array *array%s)\n",
			     $feature, $param_decl);
    } elsif ($line_fields[3] =~ /^array/) {
      $func_decl .= sprintf ("glibtop_get_%s_l (glibtop_client *client, glibtop_array *array, %s *buf%s)\n",
			     $feature, 'glibtop_'.$feature, $param_decl);
    } else {
      $func_decl .= sprintf ("glibtop_get_%s_l (glibtop_client *client, %s *buf%s)\n",
			     $feature, 'glibtop_'.$feature, $param_decl);
    }

    $total_code = sprintf ("%s{\n%s\n%s\n%s}\n", $func_decl,
			   $local_var_code, $total_code);

    print $total_code;
}
