package Libgtop;

use strict;
use vars qw($VERSION @ISA @EXPORT @EXPORT_OK);

require Exporter;
require DynaLoader;
require AutoLoader;

@ISA = qw(Exporter DynaLoader);
# Items to export into callers namespace by default. Note: do not export
# names by default without a very good reason. Use EXPORT_OK instead.
# Do not simply export all your public functions/methods/constants.
@EXPORT = qw(
	
);
$VERSION = '0.01';

bootstrap Libgtop $VERSION;

# Preloaded methods go here.

sub new {
  my $proto = shift;
  my $class = ref($proto) || $proto;
  my $self  = Libgtop::init ($class);
  return $self;
}

# Autoload methods go after __END__, and are processed by the autosplit program.

1;
__END__
