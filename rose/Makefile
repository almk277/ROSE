.PHONY: all clean clean-all depend

CFLAGS += -Wall -g
LDFLAGS += -g
CPPFLAGS += -I../lib
LIBS += -L../lib -lrose

all: rose

rose: rose.o module.o thread.o cpu.o heap.o config.o array.o object.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

cpu.o: instr.h

instr.h: ../doc/isa.dsc gen.pl cpu.c
	perl gen.pl

clean:
	rm -f *.o rose

clean-all: clean
	rm -f instr.h

depend:
	@gcc $(CPPFLAGS) -MM *.c

# DEPENDENCIES
