#!/usr/bin/perl

require 'c_types.pl';

$[ = 1;			# set array base to 1
$, = ' ';		# set output field separator
$\ = "\n";		# set output record separator

$always_use_temp_storage = 0;

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

print '/* demarshal.c */';
print "/* This is a generated file.  Please modify `demarshal.pl' */";
print '';

print '#include <glibtop.h>';
print '#include <glibtop/open.h>';
print '#include <glibtop/xmalloc.h>';
print '';
print '#include <glibtop/sysdeps.h>';
print '#include <glibtop/union.h>';
print '';
print '#include <glibtop/backend.h>';
print '';
print '#include <glibtop-backend-private.h>';
print '#include "server.h"';
print '';

$feature_count = 0;

while (<>) {
  chop;	# strip record separator

  if (/^[^\#]/) {
    &output($_);
  }
}

sub output {
  local($line) = @_;
  @line_fields = split(/\|/, $line, 9999);
  $retval = $line_fields[1];
  $element_def = $line_fields[3];
  $feature = $line_fields[2];
  $param_def = $line_fields[4];

  return if $feature =~ /^@/;
  $name = 'glibtop_'.$feature;
  
  $orig = $feature;
  $feature =~ s/^@//;
  $space = $feature;
  $space =~ s/./ /g;
  
  $features{++$feature_count} = $feature;

  $orig_retval = $retval;
  
  if ($retval eq 'retval') {
    $retval_param = '&retval';
    $call_prefix = '';
    $call_prefix_space = '';
    $prefix = 'retval = ';
    $prefix_space = '         ';
    $retval = 'int';
  } elsif ($retval !~ /^void$/) {
    $retval_param = 'NULL';
    $prefix = 'retval = ';
    $prefix_space = '         ';
    $call_prefix = 'retval = ';
    $call_prefix_space = '         ';
  } else {
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
  $first_param_name = '';
  $size_code = "\t/* send size */\n";
  $size_code .= "\t_LIBGTOP_SEND_len = 0;\n";
  
  $local_var_decl_code = '';
  $local_var_decl_code .= "\tsize_t _LIBGTOP_SEND_len, _LIBGTOP_RECV_len;\n";
  $local_var_decl_code .= "\toff_t _LIBGTOP_SEND_offset;\n";
  $local_var_decl_code .= "\tchar *_LIBGTOP_RECV_buf, *_LIBGTOP_RECV_ptr;\n";
  $local_var_decl_code .= "\tconst char *_LIBGTOP_SEND_ptr;\n";
  $local_var_decl_code .= sprintf
    (qq[\tglibtop_%s %s;\n], $feature, $feature);
  if ($retval !~ /^void$/) {
    $local_var_decl_code .= sprintf ("\t%s retval = (%s) 0;\n",
				     $retval, $retval);
  }

  $init_local_var_code = "\t/* variable initialization */\n";
  $init_local_var_code .= sprintf
    (qq[\tmemset (&%s, 0, sizeof (glibtop_%s));\n], $feature, $feature);
  $init_local_var_code .= "\t_LIBGTOP_SEND_offset = 0;\n";
  $init_local_var_code .= "\t_LIBGTOP_SEND_ptr = ".
    "(char *) send_ptr;\n";

  $demarshal_code = '';

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
      } else {
	$param_decl = $param_decl . ', ';
      }
      $param_decl = $param_decl . $c_type . ' ' . $fields[$field];
      $call_param = $call_param . ', _LIBGTOP_demarshal_' . $fields[$field];

      $size_code .= "\t_LIBGTOP_SEND_len += ";
      if ($typeinfo->{$type}->[2]) {
	$size_code .= sprintf ("sizeof (size_t)");
      } else {
	$size_code .= sprintf ("sizeof (%s)", $c_type);
      }
      $size_code .= ";\n";

      $local_var_decl_code .= sprintf
	(qq[\t%s _LIBGTOP_demarshal_%s;\n], $c_type, $fields[$field]);
      $init_local_var_code .= sprintf
	(qq[\tmemset (&_LIBGTOP_demarshal_%s, 0, sizeof (%s));\n],
	 $fields[$field], $c_type);

      $demarshal_code .= "\t_LIBGTOP_SEND_ptr = ".
	"(char *) send_ptr + _LIBGTOP_SEND_offset;\n";

      if ($typeinfo->{$type}->[2]) {
	$demarshal_code .= sprintf ("\tmemcpy (&_LIBGTOP_TEMP_len, _LIBGTOP_SEND_ptr, sizeof (size_t));\n");
	$demarshal_code .= sprintf
	  (qq[\t_LIBGTOP_DATA_len += _LIBGTOP_TEMP_len;\n]);
	$demarshal_code .= sprintf
	  (qq[\tif (_LIBGTOP_DATA_len > data_size)\n\t\treturn -GLIBTOP_ERROR_DEMARSHAL_ERROR;\n]);
	$demarshal_code .= sprintf
	  (qq[\t_LIBGTOP_DATA_offset += _LIBGTOP_TEMP_len;\n]);
      }

      if (defined $demarshal_funcs->{$type}) {
	$demarshal_code .= $demarshal_funcs->{$type}->($c_type, $fields[$field], "\t");
      } else {
	$demarshal_code .= sprintf ("\tmemcpy (&_LIBGTOP_demarshal_%s, _LIBGTOP_SEND_ptr, %s);\n",
				    $fields[$field], "sizeof ($c_type)");
      }

      $demarshal_code .= "\t_LIBGTOP_SEND_offset += ";
      if ($typeinfo->{$type}->[2]) {
	$demarshal_code .= sprintf ("sizeof (size_t)");
      } else {
	$demarshal_code .= sprintf ("sizeof (%s)", $c_type);
      }
      $demarshal_code .= ";\n";
    }
  }

  if (!($demarshal_code eq '')) {
    $demarshal_code = sprintf (qq[\t/* demarshal start */\n%s\n],
			       $demarshal_code);
  }

  if ($need_temp_storage) {
    $local_var_decl_code .= "\tsize_t _LIBGTOP_TEMP_len;\n";
    $local_var_decl_code .= "\tsize_t _LIBGTOP_DATA_len;\n";
    $local_var_decl_code .= "\toff_t _LIBGTOP_DATA_offset;\n";
    $local_var_decl_code .= "\tchar *_LIBGTOP_DATA_ptr;\n";

    $init_local_var_code .= "\t_LIBGTOP_DATA_len = 0;\n";
    $init_local_var_code .= "\t_LIBGTOP_DATA_offset = 0;\n";
    $init_local_var_code .= "\t_LIBGTOP_DATA_ptr = ".
      "(char *) data_ptr;\n";
  }

  if ($line_fields[3] eq '') {
    $call_code = sprintf
      ("\tretval = glibtop_get_%s_p (server%s);\n",
       $feature, $call_param);
  } elsif ($line_fields[3] eq 'array') {
    $call_code = sprintf
      ("\tretval = glibtop_get_%s_p (server, &array%s);\n",
       $feature, $call_param);
    $local_var_decl_code .= sprintf (qq[\tglibtop_array array;\n]);
  } elsif ($line_fields[3] =~ /^array/) {
    $call_code = sprintf
      ("\tretval = glibtop_get_%s_p (server, &array, &%s%s);\n",
       $feature, $feature, $call_param);
    $local_var_decl_code .= sprintf (qq[\tglibtop_array array;\n]);
  } else {
    $call_code = sprintf
      ("\tretval = glibtop_get_%s_p (server, &%s%s);\n",
       $feature, $feature, $call_param);
  }

  if ($orig_retval eq 'retval') {
    $call_code .= sprintf
      (qq[\tif (retval_ptr)\n\t\t*retval_ptr = retval;\n]);
  } else {
    $call_code .= sprintf
      (qq[\tif (retval_ptr)\n\t\t*retval_ptr = server->glibtop_errno;\n]);
  }

  $size_code .= sprintf
    (qq[\tif (_LIBGTOP_SEND_len != send_size)\n\t\treturn -GLIBTOP_ERROR_DEMARSHAL_ERROR;\n\n]);

  $size_code .= "\t/* recv size */\n";
  $size_code .= "\t_LIBGTOP_RECV_len = 0;\n";
  if ($line_fields[3] eq '') {
  } elsif ($line_fields[3] eq 'array') {
    $size_code .= "\t_LIBGTOP_RECV_len += sizeof (glibtop_array);\n";
  } elsif ($line_fields[3] =~ /^array/) {
    $size_code .= "\t_LIBGTOP_RECV_len += sizeof (glibtop_array);\n";
    $size_code .= sprintf
      (qq[\t_LIBGTOP_RECV_len += sizeof (glibtop_%s);\n], $feature);
  } else {
    $size_code .= sprintf
      (qq[\t_LIBGTOP_RECV_len += sizeof (glibtop_%s);\n], $feature);
  }
  $size_code .= "\n";

  $size_code .= "\t/* recv buffer */\n";
  if ($line_fields[3] eq '') {
    $size_code .= "\t_LIBGTOP_RECV_buf = NULL;\n";
  } else {
    $size_code .= "\t_LIBGTOP_RECV_buf = glibtop_malloc_r (server, _LIBGTOP_RECV_len);\n";
  }
  $size_code .= "\t_LIBGTOP_RECV_ptr = _LIBGTOP_RECV_buf;\n\n";

  $recv_buf_code = '';
  if ($line_fields[3] eq '') {
  } elsif ($line_fields[3] eq 'array') {
    $recv_buf_code = "\t/* write recv buffer */\n";
    $recv_buf_code .= sprintf
      (qq[\tmemcpy (_LIBGTOP_RECV_ptr, &array, sizeof (glibtop_array));\n]);
    $recv_buf_code .= sprintf
      (qq[\t_LIBGTOP_RECV_ptr += sizeof (glibtop_array);\n]);
  } elsif ($line_fields[3] =~ /^array/) {
    $recv_buf_code = "\t/* write recv buffer */\n";
    $recv_buf_code .= sprintf
      (qq[\tmemcpy (_LIBGTOP_RECV_ptr, &array, sizeof (glibtop_array));\n]);
    $recv_buf_code .= sprintf
      (qq[\t_LIBGTOP_RECV_ptr += sizeof (glibtop_array);\n]);
    $recv_buf_code .= sprintf
      (qq[\tmemcpy (_LIBGTOP_RECV_ptr, &%s, sizeof (glibtop_%s));\n],
       $feature, $feature);
    $recv_buf_code .= sprintf
      (qq[\t_LIBGTOP_RECV_ptr += sizeof (glibtop_%s);\n], $feature);
  } else {
    $recv_buf_code = "\t/* write recv buffer */\n";
    $recv_buf_code .= sprintf
      (qq[\tmemcpy (_LIBGTOP_RECV_ptr, &%s, sizeof (glibtop_%s));\n],
       $feature, $feature);
    $recv_buf_code .= sprintf
      (qq[\t_LIBGTOP_RECV_ptr += sizeof (glibtop_%s);\n], $feature);
  }

  $recv_buf_code .= "\n";
  $recv_buf_code .= "\t*recv_buf_ptr = _LIBGTOP_RECV_buf;\n";
  $recv_buf_code .= "\t*recv_size_ptr = _LIBGTOP_RECV_len;\n";

  $func_decl_code = sprintf
    (qq[static int\n_glibtop_demarshal_%s_i (glibtop *server, glibtop_backend *backend, const void *send_ptr, size_t send_size, void *data_ptr, size_t data_size, void **recv_buf_ptr, size_t *recv_size_ptr, int *retval_ptr)\n], $feature);

  if (!($call_code eq '')) {
    $call_code .= "\n";
  }
  
  $func_body_code = sprintf
    (qq[%s%s%s%s], $size_code, $demarshal_code, $call_code, $recv_buf_code);
  
  $total_code = sprintf
    (qq[%s{\n%s\n%s\n%s\n\treturn 0;\n}\n],
     $func_decl_code, $local_var_decl_code, $init_local_var_code,
     $func_body_code);
  
  print $total_code;
}

$func_decl_code = sprintf
  (qq[int\nglibtop_demarshal_func_i (glibtop *server, glibtop_backend *backend, unsigned command, const void *send_ptr, size_t send_size, void *data_ptr, size_t data_size, void **recv_buf_ptr, size_t *recv_size_ptr, int *retval_ptr)]);

$switch_body_code = '';

for ($nr = 1; $nr <= $feature_count; $nr++) {
  $feature = $features{$nr};

  $switch_body_code .= sprintf
    (qq[\tcase GLIBTOP_CMND_%s:\n\t\treturn _glibtop_demarshal_%s_i\n\t\t\t(server, backend, send_ptr, send_size,\n\t\t\t data_ptr, data_size,\n\t\t\t recv_buf_ptr, recv_size_ptr, retval_ptr);\n],
     &toupper ($feature), $feature);
}

$switch_code = sprintf
  (qq[\tswitch (command) {\n%s\tdefault:\n\t\treturn -GLIBTOP_ERROR_INVALID_ARGUMENT;\n\t}\n], $switch_body_code);

$total_code = sprintf
  (qq[%s\n{\n%s}\n\n], $func_decl_code, $switch_code);

print $total_code;

1;
