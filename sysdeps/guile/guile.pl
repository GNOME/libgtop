#!/usr/bin/perl

require 'guile_types.pl';

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

$feature_count = 0;

while (<>) {
  chop;				# strip record separator
  
  if (/^[^\#]/) {
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
  
  $temp_list_count = 0;
  $temp_string_count = 0;
  $have_count_var = 0;
  
  if ($retval eq 'retval') {
    $retval = 'int';
  }

  $pre_call_code = '';
  $post_call_code = '';
  
  $local_var_decl_code = sprintf (qq[\tglibtop_%s %s;\n\tSCM list;\n],
				  $feature, $feature);
  
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
      if ($type eq 'string') {
	$local_var_decl_code .= sprintf
	  (qq[\tchar *_LIBGTOP_TEMP_str%d;\n], ++$temp_string_count);

	$pre_call_code .= sprintf
	  (qq[\t_LIBGTOP_TEMP_str%d = gh_scm2newstr (%s, NULL);\n],
	   $temp_string_count, $fields[$field]);

	$post_call_code .= sprintf
	  (qq[\tfree (_LIBGTOP_TEMP_str%d);\n], $temp_string_count);

	$call_param .= sprintf
	  (qq[, _LIBGTOP_TEMP_str%d], $temp_string_count);
      } else {
	$call_param .= sprintf
	  (qq[, %s (%s)], $typeinfo->{$type}->[1], $fields[$field]);
      }
    }
  }
  if ($param_decl eq '') {
    $param_decl = 'void';
  }
  
  $nr_params_field{$feature} = $total_nr_params;
  
  $feature_name = $feature;
  $feature_name =~ s/_/-/;
  
  $scm_proc_code = sprintf
    (qq[SCM_PROC (s_%s, "glibtop-get-%s", %d, 0, 0, glibtop_guile_get_%s);],
     $feature, $feature_name, $nr_params_field{$feature}, $feature);
  
  $func_decl_code = sprintf
    (qq[static SCM\nglibtop_guile_get_%s (%s)], $feature, $param_decl);
  
  if ($retval ne 'void') {
    $local_var_decl_code .= sprintf (qq[\t%s retval;\n], $retval);
  }
  
  if ($retval ne 'void') {
    $prefix = 'retval = ';
  } else {
    $prefix = '';
  }
  
  $libgtop_call_code = sprintf
    (qq[\t%sglibtop_get_%s (&%s%s);\n\n], $prefix, $feature,
     $feature, $call_param);
  
  $temp_list_code = '';
  
  $create_list_code = sprintf
    ("\tlist = gh_list (gh_ulong2scm (%s.flags),\n\t\t\t", $feature);
  
  $nr_elements = (@elements = split(/:/, $element_def, 9999));
  for ($element = 1; $element <= $nr_elements; $element++) {
    $list = $elements[$element];
    $type = $elements[$element];
    $type =~ s/\(.*//;
    $list =~ s/^.*\(//;
    $list =~ s/\)$//;
    $count = (@fields = split(/,/, $list, 9999));
    for ($field = 1; $field <= $count; $field++) {
      if ($fields[$field] =~ /^(\w+)\[([^\]]+)\]$/) {
	@field_parts = split(/\[/, $fields[$field], 9999);
	$fields[$field] = $field_parts[1];
	$field_parts[2] =~ s/\]//;
	
	if (!$have_count_var) {
	  $local_var_decl_code .= sprintf (qq[\tlong _LIBGTOP_tmp_i;\n]);
	  $have_count_var = 1;
	}
	
	$local_var_decl_code .= sprintf
	  (qq[\tSCM _LIBGTOP_TEMP_list%d;\n], ++$temp_list_count);
	
	$temp_list_code .= sprintf
	  (qq[\t_LIBGTOP_TEMP_list%d = SCM_EOL;\n], $temp_list_count);
	
	$temp_list_code .= sprintf
	  (qq[\tfor (_LIBGTOP_tmp_i = 0; _LIBGTOP_tmp_i < %s; _LIBGTOP_tmp_i++)\n\t\t_LIBGTOP_TEMP_list%d = gh_append2\n\t\t\t(_LIBGTOP_TEMP_list%d,\n\t\t\t gh_list\n\t\t\t (%s (%s.%s [_LIBGTOP_tmp_i]),\n\t\t\t  SCM_UNDEFINED));\n\n], $field_parts[2], $temp_list_count, $temp_list_count, $typeinfo->{$type}->[0], $feature, $fields[$field]);
	
	$create_list_code .= sprintf
	  (qq[_LIBGTOP_TEMP_list%d,\n\t\t\t], $temp_list_count);
      } else {
	$create_list_code .= sprintf
	  (qq[%s (%s.%s),\n\t\t\t], $typeinfo->{$type}->[0], $feature,
	   $fields[$field]);
      }
    }
  }
  
  $create_list_code .= "SCM_UNDEFINED);\n";
  
  $total = sprintf ("%s\n\n%s\n{\n%s\n\n%s\n%s\n%s%s\n\n%s\n\treturn list;\n}\n",
		    $scm_proc_code, $func_decl_code, $local_var_decl_code,
		    $pre_call_code, $libgtop_call_code, $post_call_code,
		    $temp_list_code, $create_list_code);
  
  print $total;
}
