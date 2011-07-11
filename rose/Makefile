.PHONY		: all clean debug

CPPFLAGS += -I../include
CXXFLAGS += -Wall -g

all			: rose

rose		: main.o module.o
	$(CXX) $(LDFLAGS) -o $@ $^

isa.o		: isa.cpp isa.h
isa.h		: isa_decl.h
opcode.o	: opcode.cpp opc_table.c

clean		:
	$(RM) *.o rose isa_decl.h opc_table.c tags

debug		: CXXFLAGS += -g
debug		: LDFLAGS += -g
debug		: rose

tags		: *.cpp
	ctags *.cpp

isa_decl.h	: isa.cpp
	perl instr.pl

opc_table.c	: isa.cpp
	perl instr.pl

