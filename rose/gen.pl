#! /usr/bin/perl -w
use strict;

# This script reads ROSE ISA documentation file and generates instruction names

my $dsc_name = "../doc/isa.dsc";
my $cpu_name = "cpu.c";
my $opcodes_name = "instr.h";
my $op_enum_name = "Instuctions";
my %name2code;
my %codedefined;
my %codeused;
my (@names, @nonimplemented);

# let's see what opcodes are used already
open(CPU, "$cpu_name") or die("$cpu_name: $!\n");
while(<CPU>) {
# opcodes should be used in a form: "case I_foo:"
	/case I_(\w+)\:/ and $codeused{$1} = 1;
}
close(CPU);

open(DSC, "$dsc_name") or die("$dsc_name: $!\n");
open(OP, ">$opcodes_name") or die("$opcodes_name: $!\n");
print OP "/* auto-generated; do not edit */\n";
print OP "enum $op_enum_name {\n";
while(<DSC>) {
	next if /^#/; # skip comments
	next unless /\S/; # skip empty lines
	chomp;
	if(/([\w\.]+)\s+([\w-])+\s+([\dA-F]{2})\s+(.*)$/) {
		my $name = $1; #instruction name
		my $code = $3; # instruction code string
		defined($codedefined{$code}) and die "opcode $code defined twice\n";
		$codedefined{$code} = 1;
		$name =~ tr/\./_/;
		print OP "\tI_$name = 0x$code,\n";
		defined($codeused{$name}) or push @nonimplemented, $name;
	} else {
		die "$dsc_name: Can not parse string: '$_'\n";
	}
}
print OP "};\n\n";
close(OP) or die("$opcodes_name: $!\n");
close(DSC);

@nonimplemented and print "Warning! " .
	"The following instructions are not implemented: @nonimplemented\n"
