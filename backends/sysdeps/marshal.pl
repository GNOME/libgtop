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

    $features{++$feature_count} = $orig;

    return if $orig =~ /^@/;

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
	  $param_decl = ",\n             " . $space . '    ';
	}
	else {
	  $param_decl = $param_decl . ', ';
	}
	$param_decl = $param_decl . $c_type . ' ' . $fields[$field];
	$call_param = $call_param . ', ' . $fields[$field];
      }
    }

    if ($line_fields[3] eq '') {
      $total_code = sprintf
	("\treturn glibtop_get_%s_s (server%s);\n", $feature, $call_param);
    } elsif ($line_fields[3] eq 'array') {
      $total_code = sprintf
	("\treturn glibtop_get_%s_s (server, array%s);\n", $feature, $call_param);
    } elsif ($line_fields[3] =~ /^array/) {
      $total_code = sprintf
	("\treturn glibtop_get_%s_s (server, array, buf%s);\n", $feature, $call_param);
    } else {
      $total_code = sprintf
	("\treturn glibtop_get_%s_s (server, buf%s);\n", $feature, $call_param);
    }

    $func_decl = 'static '.$retval."\n";
    if ($line_fields[3] eq '') {
      $func_decl .= sprintf ("_glibtop_get_%s_c (glibtop_server *server, glibtop_backend *backend%s)\n",
			     $feature, $param_decl);
    } elsif ($line_fields[3] eq 'array') {
      $func_decl .= sprintf ("_glibtop_get_%s_c (glibtop_server *server, glibtop_backend *backend, glibtop_array *array%s)\n",
			     $feature, $param_decl);
    } elsif ($line_fields[3] =~ /^array/) {
      $func_decl .= sprintf ("_glibtop_get_%s_c (glibtop_server *server, glibtop_backend *backend, glibtop_array *array, %s *buf%s)\n",
			     $feature, 'glibtop_'.$feature, $param_decl);
    } else {
      $func_decl .= sprintf ("_glibtop_get_%s_c (glibtop_server *server, glibtop_backend *backend, %s *buf%s)\n",
			     $feature, 'glibtop_'.$feature, $param_decl);
    }

    $total_code = sprintf ("%s{\n%s\n%s}\n", $func_decl, $total_code);

    $total_code = sprintf ("#if (!GLIBTOP_SUID_%s)\n\n%s\n#endif /* not GLIBTOP_SUID_%s */\n\n",
			   &toupper($feature), $total_code,
			   &toupper($feature));

    print $total_code;
}

$init_hook_code = '';
$call_vector_code = '';
for ($nr = 1; $nr <= $feature_count; $nr++) {
  $feature = $features{$nr};

  if ($feature =~ /^@/) {
    $call_vector_code .= sprintf (qq[\tNULL,\n]);
  } else {
    $call_vector_code .= sprintf
      (qq[\#if GLIBTOP_SUID_%s\n\tNULL,\n\#else\n\t_glibtop_get_%s_c,\n\#endif\n],
       &toupper($feature), $feature);
  }

  if (!($feature =~ /^@/)) {
    $init_hook_code .= sprintf
      (qq[\#if !GLIBTOP_SUID_%s\n\tglibtop_init_%s_s,\n\#endif\n],
       &toupper($feature), $feature);
  }
}
$init_hook_code .= sprintf (qq[\tNULL\n]);

chop $init_hook_code;
chop $call_vector_code;

print 'glibtop_call_vector glibtop_backend_sysdeps_call_vector = {';
print $call_vector_code;
print '};';
print '';

print 'glibtop_init_func_t _glibtop_init_hook_s [] = {';
print $init_hook_code;
print '};';
print '';

