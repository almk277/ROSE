#! /bin/sh

# This extracts opcodes from source file

FILE=opcode.cpp

sed -n '/opcodes\[\]/,/\}\;/p' $FILE | grep '/\*' | grep -v ERROR |
	sed 's/isa_//' | sed 's/\,//'
