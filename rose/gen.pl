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
	NOT_IMPLEMENTED(\"$name\");
}

";
}

my $dsc_name = "../doc/isa.dsc";
my $func_name = "isa_desc.c";
my $tbl_name = "isa_tbl.c";
my $dbg_name = "isa_dbg.c";
my $dbgtbl_name = "dbg_tbl.c";
my $disas_name = "disas_tbl.c";
my (%funclist, %dbglist);
my (@names, @codes);

open(FUNC, "$func_name") or die("$func_name: $!\n");
while(<FUNC>) {
	/isa_(\w+)\s*\(/ and $funclist{$1} = 1;
}
close(FUNC);

open(DBG, "$dbg_name") or die("$dbg_name: $!\n");
while(<DBG>) {
	/dbg_(\w+)\s*\(/ and $dbglist{$1} = 1;
}
close(DBG);

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
		defined($names[$idx]) and die "$1 and $names[$idx] have opcode $idx";
		$names[$idx] = $1;
		$codes[$idx] = $2;
		if(!defined($funclist{$1})) {
			print FUNC func_stub($1, "[$2] $4");
		}
	} else {
		die "$dsc_name: Can not parse string: $_\n";
	}
}
close(DSC);
close(FUNC) or die("$func_name: $!\n");

# Create instruction and debug tables
open(TBL, ">$tbl_name") or die("$tbl_name: $!\n");
open(DBG, ">$dbgtbl_name") or die("$dbgtbl_name: $!\n");
open(DIS, ">$disas_name") or die("$disas_name: $!\n");
for(my $i = 0; $i != 256; ++$i) {
	my ($out, $dbg, $name, $code);
	if(defined($names[$i])) {
		$out = "isa_$names[$i]";
		$name = "\"$names[$i]\"";
		$code = "OPER_$codes[$i]";
		if(defined($dbglist{$names[$i]})) {
			$dbg = "dbg_$names[$i]";
		} else {
			$dbg = $out;
		}
	} else {
		$out = "NO_INSTR";
		$dbg = "DBG_NO_INSTR";
		$name = "DIS_NO_INSTR";
		$code = "OPER_none";
	}
	printf TBL "%-15s /* 0x%.2X */\n", $out . ",", $i;
	printf DBG "%-15s /* 0x%.2X */\n", $dbg . ",", $i;
	printf DIS "{ %-15s %-12s }, /* 0x%.2X */\n", $name . ",", $code, $i;
}
close(DIS) or die("$disas_name: $!\n");
close(DBG) or die("$tbl_name: $!\n");
close(TBL) or die("$dbg_name: $!\n");

