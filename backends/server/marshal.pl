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

print '/* marshal.c */';
print "/* This is a generated file.  Please modify `marshal.pl' */";
print '';

print '#include <glibtop.h>';
print '#include <glibtop/open.h>';
print '#include <glibtop/xmalloc.h>';
print '';
print '#include <glibtop/sysdeps.h>';
print '#include <glibtop/union.h>';
print '';
print '#include <glibtop/command.h>';
print '#include <glibtop/backend.h>';
print '';
print '#include <glibtop-backend-private.h>';
print '';

$feature_count = 0;

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

    $features{++$feature_count} = $feature;

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

    $total_code = '';

    $send_buf_code = "\t_LIBGTOP_SEND_buf = ";
    if ($need_temp_storage) {
      $send_buf_code .= "glibtop_malloc_r (server, _LIBGTOP_SEND_len+1)";
    } else {
      $send_buf_code .= '(void *) &'.$first_param_name;
    }
    $send_buf_code .= ";\n";

    $call_code = '';
    $call_code .= sprintf ("\t%sglibtop_call_i (server, backend, GLIBTOP_CMND_%s,\n",
			   $call_prefix, &toupper($feature));
    $call_code .= sprintf ("\t\t\t%s%s, %s,\n", $call_prefix_space,
			   "_LIBGTOP_SEND_len", "_LIBGTOP_SEND_buf");
    if ($line_fields[3] eq '') {
      $call_code .= sprintf ("\t\t\t%s0, NULL,\n", $call_prefix_space);
    } elsif ($line_fields[3] eq  'array') {
      $call_code .= sprintf ("\t\t\t%ssizeof (glibtop_array), array,\n",
			     $call_prefix_space);
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

    $total_code .= sprintf ("%s\n%s\n%s\n%s\n%s\n",
			    $size_code, $send_buf_code, $marshal_code,
			    $call_code, $send_buf_free_code);

    $return_code = '';
    if ($retval !~ /^void$/) {
      $return_code .= "\treturn retval;\n";
    } else {
      $return_code .= "\treturn;\n";
    }

    $total_code .= $return_code;

    $func_decl = 'static '.$retval."\n";
    if ($line_fields[3] eq '') {
      $func_decl .= sprintf ("_glibtop_get_%s_c (glibtop *server, glibtop_backend *backend%s)\n",
			     $feature, $param_decl);
    } elsif ($line_fields[3] eq 'array') {
      $func_decl .= sprintf ("_glibtop_get_%s_c (glibtop *server, glibtop_backend *backend, glibtop_array *array%s)\n",
			     $feature, $param_decl);
    } elsif ($line_fields[3] =~ /^array/) {
      $func_decl .= sprintf ("_glibtop_get_%s_c (glibtop *server, glibtop_backend *backend, glibtop_array *array, %s *buf%s)\n",
			     $feature, 'glibtop_'.$feature, $param_decl);
    } else {
      $func_decl .= sprintf ("_glibtop_get_%s_c (glibtop *server, glibtop_backend *backend, %s *buf%s)\n",
			     $feature, 'glibtop_'.$feature, $param_decl);
    }

    $total_code = sprintf ("%s{\n%s\n%s\n%s}\n", $func_decl,
			   $local_var_code, $total_code);


    $total_code = sprintf ("#if GLIBTOP_SUID_%s\n\n%s\n#endif /* GLIBTOP_SUID_%s */\n\n",
			   &toupper($feature), $total_code,
			   &toupper($feature));

    print $total_code;
}

$call_vector_code = '';
for ($nr = 1; $nr <= $feature_count; $nr++) {
  $feature = $features{$nr};

  $call_vector_code .= sprintf
    (qq[\#if GLIBTOP_SUID_%s\n\t_glibtop_get_%s_c,\n\#else\n\tNULL,\n\#endif\n],
     &toupper($feature), $feature);
}

print 'glibtop_call_vector glibtop_backend_server_call_vector = {';
print $call_vector_code;
print '};';
print '';

