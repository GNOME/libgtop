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

print '/* backend-kernel.h */';
print "/* This is a generated file.  Please modify `backend-kernel.pl' */";
print '';

print '#ifndef __GLIBTOP_BACKEND_KERNEL_H__';
print '#define __GLIBTOP_BACKEND_KERNEL_H__';
print '';
print '#include <glibtop.h>';
print '#include <glibtop/union.h>';
print '';
print 'BEGIN_LIBGTOP_DECLS';
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
	  $param_decl = ",\n            " . $space . '    ';
	}
	else {
	  $param_decl = $param_decl . ', ';
	}
	$param_decl = $param_decl . $c_type . ' ' . $fields[$field];
	$call_param = $call_param . ', ' . $fields[$field];
      }
    }

    $func_decl = sprintf ("int\nglibtop_init_%s_k (glibtop *server);\n\n",
			  $feature);

    $func_decl .= $retval."\n";
    if ($line_fields[3] eq '') {
      $func_decl .= sprintf ("glibtop_get_%s_k (glibtop *server%s);\n",
			     $feature, $param_decl);
    } elsif ($line_fields[3] eq 'array') {
      $func_decl .= sprintf ("glibtop_get_%s_k (glibtop *server, glibtop_array *array%s);\n",
			     $feature, $param_decl);
    } elsif ($line_fields[3] =~ /^array/) {
      $func_decl .= sprintf ("glibtop_get_%s_k (glibtop *server, glibtop_array *array, %s *buf%s);\n",
			     $feature, 'glibtop_'.$feature, $param_decl);
    } else {
      $func_decl .= sprintf ("glibtop_get_%s_k (glibtop *server, %s *buf%s);\n",
			     $feature, 'glibtop_'.$feature, $param_decl);
    }

    $total_code = $func_decl;

    print $total_code;
}

print '';
print 'END_LIBGTOP_DECLS';
print '';
print '#endif';
print '';
