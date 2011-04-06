#! /usr/bin/perl -w

# This script reads ROSE ISA table from standard input and writes it to
# standard output in gperf format.

use strict;

while(<STDIN>) {
	next if /^#/; # skip comments
	next unless /\S/; # skip empty lines
	chomp;
	while(/\\$/) {
		# restore broken line
		s/\\$//;
		my $rest = <STDIN>;
		$rest =~ s/^\s*//;
		chomp $rest;
		$_ .= $rest;
	}
	if(/^(\w+)\s+(\w+)\s+(\w+)\s+(.*)$/) {
		print "$1, ROSE_ITYPE_$2, $3, \"$4\"\n";
	} else {
		die "Could not parse this line: '$_'"
	}
}

