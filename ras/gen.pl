#! /usr/bin/perl -w
use strict;

# This script reads ROSE ISA table, generates unneccessary code
# and writes it to some files.

my $oper_name = "../doc/operand.dsc";
my $isa_name = "../doc/isa.dsc";
my $lex_name = "ras-lex.l";
my $lex_head_name = "ras-head.l";
my $lex_tail_name = "ras-tail.l";
my %arglens;

sub copy_file
{
	my $src = shift;
	local *DST = shift;
	open(SRC, "$src") or die("$src: $!\n");
	while(<SRC>) { print DST $_; }
	close SRC;
}

open(OPER, "$oper_name") or die("$oper_name: $!\n");
while(<OPER>) {
	next if /^#/;
	next unless /\S/;
	if (/^([\w-])\s+(\d+)/) {
		defined $arglens{$1} and die("$oper_name: $1: defined already\n");
		$arglens{$1} = $2;
	} else {
		die("$oper_name: cannot parse: $_");
	}
}
close(OPER);

open(LEX, ">$lex_name") or die("$lex_name: $!\n");

copy_file("$lex_head_name", *LEX);

open(ISA, "$isa_name") or die("$isa_name: $!\n");
while(<ISA>) {
	next if /^#/; # skip comments
	next unless /\S/; # skip empty lines
	if(/^([\w\.]+)\s+([\w-]+)\s+(\w+)\s+(.*)$/) {
		my $instrlen = 1;
		for(my $i = 0; $i != length $2; ++$i) {
			my $c = substr $2, $i, 1;
			my $len = $arglens{$c};
			if(not defined $len) { die("Unknown argument code: $c\n"); }
			$instrlen += $len;
		}
		printf LEX "%-10s emit_instruction($instrlen, $3, \"$2\"); \n", $1
	} else {
		die "Could not parse this line: '$_'"
	}
}
close(ISA);

copy_file("$lex_tail_name", *LEX);

close(LEX) or die("$lex_name: $!\n");
