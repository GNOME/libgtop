#!/usr/bin/perl -w

require 5.004;

use blib;
use strict;
use Libgtop;

print "CPU Usage: ".join (':', Libgtop::cpu)."\n";
print "Memory Usage: ".join (':', Libgtop::mem)."\n";
print "Swap Usage: ".join (':', Libgtop::swap)."\n";


