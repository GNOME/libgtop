#!/usr/bin/perl

require 'guile_types.pl';

die "Usage: $0 features.def structures.def" unless $#ARGV == 2;

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

print '#include "guile.h"';
print '#include "structures.h"';
print '';

$feature_count = 0;
$smob_count = 0;

$smobs{$smob_count++} = 'glibtop';

open FEATURESDEF, $ARGV[1] or
  die "open ($ARGV[1]): $!";

while (<FEATURESDEF>) {
  chop;				# strip record separator
  
  if (/^[^\#]/) {
    &make_output ($_);
  }
}

close FEATURESDEF;

open STRUCTDEF, $ARGV[2] or
  die "open ($ARGV[2]): $!";

while (<STRUCTDEF>) {
  chop;				# strip record separator
  
  if (/^[^\#]/) {
    &parse_structure_def ($_);
  }
}

close STRUCTDEF;

$init_smobs_code = sprintf
  (qq[\tscm_glibtop_smob_tags [GLIBTOP_STRUCTURE_GLIBTOP] = scm_make_smob_type\n\t\t("glibtop", sizeof (glibtop));\n]);

for ($nr = 0; $nr < $smob_count; $nr++) {
  $smob = $smobs{$nr};

  $init_smobs_code .= sprintf
    (qq[\tscm_glibtop_smob_tags [GLIBTOP_STRUCTURE_%s] = scm_make_smob_type\n\t\t("%s", sizeof (%s));\n],
     toupper($smob), $smob, $smob);
}

open OUTPUT, "> $ARGV[3]" or
  die "open ($ARGV [3]): $!";
select OUTPUT;

print qq[/* structures.h */];
print qq[/* This is a generated file.  Please modify \`guile.pl\' */];
print '';
print qq[\#ifndef __GLIBTOP_STRUCTURES_H__];
print qq[\#define __GLIBTOP_STRUCTURES_H__];
print '';
print qq[\#include <glibtop.h>];
print '';
print qq[BEGIN_LIBGTOP_DECLS];
print '';

for ($nr = 0; $nr < $smob_count; $nr++) {
  $smob = $smobs{$nr};

  printf (qq[\#define %-40s\t%d\n], 'GLIBTOP_STRUCTURE_'.&toupper($smob), $nr);
}

print '';
print qq[END_LIBGTOP_DECLS];
print '';
print qq[\#endif /* __GLIBTOP_STRUCTURES_H__ */];

close OUTPUT;

select STDOUT;

print qq[void\n];
print qq[glibtop_boot_guile (void)\n];
print '{';
print qq[#ifndef SCM_MAGIC_SNARFER\n];
print qq[#include "guile.x"\n];
print qq[#endif\n\n];
print $init_smobs_code;

print '';
printf qq[\tglibtop_boot_guile_main ();];
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

  $smobs{$smob_count++} = sprintf (qq[glibtop_%s], $feature);
  
  $total_nr_params = 0;
  
  $temp_string_count = 0;
  $have_count_var = 0;
  
  $pre_call_code = '';
  $post_call_code = '';
  
  $local_var_decl_code = sprintf (qq[\tglibtop_%s %s;\n], $feature, $feature);
  $local_var_decl_code .= sprintf (qq[\tSCM smob_answer;\n]);
  $local_var_decl_code .= sprintf (qq[\tglibtop *server;\n]);

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
  if (!($param_decl eq '')) {
    $param_decl = ', '.$param_decl;
  }
  
  $nr_params_field{$feature} = $total_nr_params;
  
  $feature_name = $feature;
  $feature_name =~ s/_/-/;

  $field_list_code = '';
  
  $init_server_code = sprintf
    (qq[\tSCM_ASSERT ((SCM_FALSEP (server_smob) ||\n\t\t    (SCM_NIMP (server_smob)\n\t\t     && (SCM_CAR (server_smob) ==\n\t\t\t scm_glibtop_smob_tags [GLIBTOP_STRUCTURE_GLIBTOP]))),\n\t\t    server_smob, SCM_ARG1, "glibtop-get-%s");\n\n], $feature_name);

  $init_server_code .= sprintf
    (qq[\tserver = SCM_FALSEP (server_smob) ? glibtop_global_server :\n\t\t(glibtop *) SCM_SMOB_DATA (server_smob);\n\n]);
  
  $scm_proc_code = sprintf
    (qq[SCM_PROC (s_%s, "glibtop-get-%s", %d, 0, 0, glibtop_guile_get_%s);],
     $feature, $feature_name, $nr_params_field{$feature}+1, $feature);

  $func_decl_code = sprintf
    (qq[static SCM\nglibtop_guile_get_%s (SCM server_smob%s)], $feature, $param_decl);
  
  if ($retval =~ /^array\((.*)\)$/) {
    $retval_type = "$1 *";
  } elsif ($retval eq 'retval') {
    $retval_type = 'int';
  } else {
    $retval_type = $retval;
  }

  if ($retval ne 'void') {
    $local_var_decl_code .= sprintf (qq[\t%s retval;\n], $retval_type);
  }
  
  if ($retval ne 'void') {
    $prefix = 'retval = ';
  } else {
    $prefix = '';
  }

  $libgtop_call_code = sprintf
    (qq[\t%sglibtop_get_%s_l (server, &%s%s);\n\n], $prefix, $feature,
     $feature, $call_param);

  if ($retval eq 'retval') {
    $check_retval_code = sprintf
      (qq[\tif (retval < 0)\n\t\treturn SCM_BOOL_F;\n]);
  } else {
    $check_retval_code = '';
  }

  $make_smob_code = sprintf
    (qq[\tsmob_answer = scm_make_smob\n\t\t(scm_glibtop_smob_tags [GLIBTOP_STRUCTURE_GLIBTOP_%s]);\n], toupper($feature));

  $make_smob_code .= sprintf
    (qq[\t*(glibtop_%s *) SCM_SMOB_DATA (smob_answer) = %s;\n\n],
     $feature, $feature);

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
	
	$field_name = $field_parts[0];
      } else {
	$field_name = $fields[$field];
      }

      $field_list_code .= sprintf
	(qq[gh_symbol2scm ("%s"), \\\n\t\t\t\t], $field_name);
    }
  }
  
  $scm_fields_code = sprintf
    (qq[SCM_GLOBAL_VCELL_INIT (s_%s_names, "glibtop-fields-%s", \\\n\t\t       gh_list (%sSCM_UNDEFINED));],
    $feature, $feature_name, $field_list_code);

  $return_smob_code = sprintf
    (qq[\treturn smob_answer;]);
  
  $total = sprintf ("%s\n\n%s\n\n%s\n{\n%s\n\n%s\n%s\n%s%s\n\n%s\n%s\n%s\n%s\n}\n",
		    $scm_proc_code, $scm_fields_code, $func_decl_code,
		    $local_var_decl_code, $init_server_code,
		    $pre_call_code, $libgtop_call_code, $post_call_code,
		    $check_retval_code, $make_smob_code, $return_smob_code);
  
  print $total;
}

sub parse_structure_def {
  local($line) = @_;
  @line_fields = split(/\|/, $line, 9999);
  $name = $line_fields[1];

  $smobs{$smob_count++} = $name;
}
