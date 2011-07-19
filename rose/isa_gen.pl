#! /usr/bin/perl -w

# This script reads ROSE ISA documentation file and generates C sources:
#   isa_desc.c is ISA implementation;
#   isa_tbl.c is ISA table.

use strict;

# Returns a string with a C-function stub
# Arguments are instruction name and comment.
sub func_stub
{
	my ($name, $comment) = @_;
	return
"/* $comment */
static void isa_$name(Thread *t)
{
	NOT_IMPLEMENTED;
}

";
}

my $dsc_name = "../doc/isa.dsc";
my $func_name = "isa_desc.c";
my $tbl_name = "isa_tbl.c";
my %funclist;
my @names;

open(FUNC, "$func_name") or die("$func_name: $!\n");
while(<FUNC>) {
	/isa_(\w+)\s*\(/ and $funclist{$1} = 1;
}
close(FUNC);

open(FUNC, ">>$func_name") or die("$func_name: $!\n");
open(DSC, "$dsc_name") or die("$dsc_name: $!\n");
while(<DSC>) {
	next if /^#/; # skip comments
	next unless /\S/; # skip empty lines
	while(/\\$/) { # concatenate strings
		chomp; chop;
		my $next = <DSC>;
		$next =~ s/^\s*//;
		$_ .= $next;
	}
	if(/(\w+)\s+(\w)\s+(0[xX][\dA-F]{2})\s+(.*)$/) {
		my $idx = hex $3; # instruction code
		$names[$idx] = $1;
		if(!defined($funclist{$1})) {
			print FUNC func_stub($1, $4);
		}
	} else {
		die "$dsc_name: Can not parse string: $_\n";
	}
}
close(DSC);
close(FUNC) or die("$func_name: $!\n");

open(TBL, ">$tbl_name") or die("$tbl_name: $!\n");
for(my $i = 0; $i != 256; ++$i) {
	my $out;
	if(defined($names[$i])) {
		$out = "isa_$names[$i]";
	} else {
		$out = "NO_INSTR";
	}
	printf TBL "%-15s /* 0x%.2X */\n", $out . ",", $i;
}
close(TBL) or die("$tbl_name: $!\n");

