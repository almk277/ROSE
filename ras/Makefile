DESCR := ../doc/isa.dsc
CPPFLAGS += -I../include

all: ras

ras: main.o storage.o ras-lex.o ../lib/symtbl.o ../lib/mm.o ../lib/symbol.o sect.o print.o
	$(CC) $(LDFLAGS) -o $@ $^

ras-lex.l: ras-head.l ras-tail.l gen.pl $(DESCR)
	perl gen.pl

debug:   CPPFLAGS += -DDEBUG
debug:   CFLAGS += -Wall -g
debug:   ras
prepare: ras-lex.c

clean:
	rm -f *.o ras

clean-all: clean
	rm -f ras-lex.l ras-lex.c

tags: *.c
	ctags *.c
