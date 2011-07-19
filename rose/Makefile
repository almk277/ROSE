.PHONY: all clean

CFLAGS += -Wall -g
CPPFLAGS += -I../include

all: rose

rose: rose.o module.o debug.o thread.o isa.o stack.o text.o \
ptbl.o exp.o sym.o isa.o

isa.c: isa_desc.c isa_tbl.c

isa_desc.c: ../doc/isa.dsc
	./isa_gen.pl

isa_tbl.c: ../doc/isa.dsc
	./isa_gen.pl

clean:
	rm -f *.o rose

clean-all: clean
	rm -f isa_tbl.c

