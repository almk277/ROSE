.PHONY: all clean clean-all depend

CFLAGS += -Wall -g
CPPFLAGS += -I../include -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include
LDFLAGS += -lglib-2.0

all: rose

rose: rose.o module.o debug.o thread.o stack.o ref.o error.o array.o disas.o

isa_desc.c: ../doc/isa.dsc
	./isa_gen.pl
	touch $@

isa_tbl.c: ../doc/isa.dsc
	./isa_gen.pl
	touch $@

dbg_tbl.c: ../doc/isa.dsc
	./isa_gen.pl
	touch $@

disas_tbl.c: ../doc/isa.dsc
	./isa_gen.pl
	touch $@

clean:
	rm -f *.o rose

clean-all: clean
	rm -f isa_tbl.c

depend:
	@gcc $(CPPFLAGS) -MM *.c

# DEPENDENCIES
array.o: array.c array.h object.h error.h ref.h
debug.o: debug.c debug.h ../include/compiler.h segment.h \
 ../include/rmd.h
error.o: error.c error.h stack.h debug.h ../include/compiler.h \
 segment.h ../include/rmd.h conf.h
isa.o: isa.c thread.h stack.h debug.h ../include/compiler.h segment.h \
 ../include/rmd.h conf.h module.h ref.h array.h object.h error.h \
 isa_desc.c isa_tbl.c
isa_desc.o: isa_desc.c
isa_tbl.o: isa_tbl.c
module.o: module.c module.h segment.h ../include/rmd.h error.h
ref.o: ref.c ref.h ../include/compiler.h error.h conf.h
rose.o: rose.c module.h segment.h ../include/rmd.h thread.h stack.h \
 debug.h ../include/compiler.h conf.h
stack.o: stack.c stack.h debug.h ../include/compiler.h segment.h \
 ../include/rmd.h conf.h
thread.o: thread.c thread.h stack.h debug.h ../include/compiler.h \
 segment.h ../include/rmd.h conf.h module.h isa.c ref.h array.h \
 object.h error.h isa_desc.c isa_tbl.c
