#!/usr/bin/perl -w

require 5.004;

use blib;
use strict;
use Libgtop;

my $server = Libgtop->new;

print "CPU Usage:    ".join (' ', $server->cpu)."\n";
print "Memory Usage: ".join (' ', $server->mem)."\n";
print "Swap Usage:   ".join (' ', $server->swap)."\n";

print "Process List: ".join (' ', $server->proclist (0,0))."\n";

