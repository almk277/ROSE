.PHONY: all clean clean-all depend

CFLAGS += -Wall -g
LDFLAGS += -g
CPPFLAGS += -I../include
LIBS += -L../lib -lrose

all: rose

rose: rose.o module.o thread.o cpu.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

cpu.o: instr.h

instr.h: ../doc/isa.dsc gen.pl
	perl gen.pl

clean:
	rm -f *.o rose

clean-all: clean
	rm -f instr.h

depend:
	@gcc $(CPPFLAGS) -MM *.c

# DEPENDENCIES
cpu.o: cpu.c thread.h ../include/rmd.h segment.h instr.h
debug.o: debug.c debug.h ../include/compiler.h segment.h \
 ../include/rmd.h
error.o: error.c error.h
module.o: module.c module.h ../include/rmd.h segment.h ../include/mm.h \
 ../include/compiler.h ../include/symbol.h ../include/symtbl.h \
 ../include/queue.h ../include/loader.h
rose.o: rose.c module.h ../include/rmd.h segment.h thread.h \
 ../include/symbol.h ../include/compiler.h cpu.h ../include/loader.h
thread.o: thread.c thread.h ../include/rmd.h segment.h module.h
