#!/usr/bin/perl -w

require 5.004;
use strict;

print "Enter port the server should listen on [42800]: ";

my $port = <stdin>; chop $port;
$port = 42800 unless $port =~ /^\d+$/;

print "\nUser name or UID to run as [nobody]: ";

my $user = <stdin>; chop $user; $user = 'nobody' if $user eq '';

my ($login, $pass, $uid, $gid);

unless ($user =~ /^\d+$/) {
    ($login, $pass, $uid, $gid) = getpwnam ($user) or
	die "User '$user' not in passwd file.";
}

my $g_default = (defined $gid) ? $gid : 'nogroup';

print "Group name or GID to run as [$g_default]: ";

my $group = <stdin>; chop $group; $group = $g_default if $group eq '';

unless ($group =~ /^\d+$/) {
    $gid = getgrnam ($group) or
	die "Group '$group' not in group file.";
}

print "\nEnter list of hosts which should be authorized to";
print "\nconnect to the server (terminate with a blank line):\n\n";

print "SECURITY WARNING:\n";
print "  Enabling access for a particular hosts means the ALL USERS on this host will\n";
print "  be allowed to connect to the server !\n\n";

print "  If you want security, let this table empty and use the 'xauth' method instead.\n";
print "  Look at the manpage of gnuserv (1) as shipped with GNU Emacs for more details\n";
print "  about security. The server uses the same security mechanisms like gnuserv from\n";
print "  XEmacs 20.3\n\n";

my @hosts = ();
my @host_addrs = ();
my @host_names = ();

while (1) {
    print "Host: ";

    my $host = <stdin>; chop $host; 
    last if $host eq '';

    my ($name,$aliases,$addrtype,$length,@addrs) = gethostbyname ($host) or
	die "gethostbyname (): Can't resolve '$host'";
    
    my ($a,$b,$c,$d) = unpack('C4',$addrs[0]);

    push @hosts, sprintf ("0x%02X%02X%02X%02X", $d, $c, $b, $a);
    push @host_addrs, sprintf ("%d.%d.%d.%d", $a, $b, $c, $d);
    push @host_names, $name;
};

print "\n";
print "This is your config:\n";
print "====================\n\n"; 

printf qq[%-30s: %d\n\n], 'Port', $port;
printf qq[%-30s: %d\n], 'UID', $uid;
printf qq[%-30s: %d\n\n], 'GID', $gid;

foreach (0..$#hosts) {
    printf qq[%-30s (%s - %s)\n], $host_names[$_], $hosts[$_], $host_addrs [$_];
}

print "\n";

print "Accept? (yes/no) ";

my $accept = <stdin>; chop $accept;

exit unless $accept eq 'yes';

print "\n";

open CONFIG, "> server_config.h" or
    die "open (server_config.h): $!";
select CONFIG;

printf qq[\#define SERVER_PORT\t\t%d\n\n], $port;

printf qq[\#define SERVER_UID\t\t%d\n], $uid;
printf qq[\#define SERVER_GID\t\t%d\n\n], $gid;

printf qq[\#define HOST_TABLE_ENTRIES\t%d\n\n], $#hosts + 1;

foreach (@host_names) {
    $_ = qq["$_"];
}

printf qq[const char *permitted_host_names [HOST_TABLE_ENTRIES] = \n];
printf qq[{ %s };\n\n], join (', ', @host_names);

printf qq[unsigned long permitted_hosts [HOST_TABLE_ENTRIES];\n];

close CONFIG;

select STDOUT;

print "Your config has successfully been written to 'server_config.h'.\n";
