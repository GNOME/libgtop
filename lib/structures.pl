#!/usr/bin/perl

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

$structures{$structure_count++} = 'glibtop';

open FEATURESDEF, $ARGV[1] or
  die "open ($ARGV[1]): $!";

while (<FEATURESDEF>) {
  chop;				# strip record separator
  
  if (/^[^\#]/) {
    &parse_features_def ($_);
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

$init_structures_code = sprintf
  (qq[\tscm_glibtop_structure_tags [GLIBTOP_STRUCTURE_GLIBTOP] = scm_make_structure_type\n\t\t("glibtop", sizeof (glibtop));\n]);

for ($nr = 0; $nr < $structure_count; $nr++) {
  $structure = $structures{$nr};

  $init_structures_code .= sprintf
    (qq[\tscm_glibtop_structure_tags [GLIBTOP_STRUCTURE_%s] = scm_make_structure_type\n\t\t("%s", sizeof (%s));\n],
     toupper($structure), $structure, $structure);
}

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

for ($nr = 0; $nr < $structure_count; $nr++) {
  $structure = $structures{$nr};

  printf (qq[\#define %-40s\t%d\n], 'GLIBTOP_STRUCTURE_'.&toupper($structure), $nr);
}

print '';
printf (qq[\#define %-40s\t%d\n], 'GLIBTOP_MAX_STRUCTURES', $structure_count);
print '';
print qq[END_LIBGTOP_DECLS];
print '';
print qq[\#endif /* __GLIBTOP_STRUCTURES_H__ */];

sub parse_features_def {
  local($line) = @_;
  @line_fields = split(/\|/, $line, 9999);
  $retval = $line_fields[1];
  $element_def = $line_fields[3];
  $feature = $line_fields[2];
  $param_def = $line_fields[4];
  
  $feature =~ s/^@//;
  $features{$feature} = $feature;

  $structures{$structure_count++} = $feature;
}

sub parse_structure_def {
  local($line) = @_;
  @line_fields = split(/\|/, $line, 9999);
  $name = $line_fields[1];

  $structures{$structure_count++} = $name;
}
