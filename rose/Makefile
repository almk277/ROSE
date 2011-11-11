.PHONY: all clean clean-all depend

CFLAGS += -Wall -g
CPPFLAGS += -I../common

all: rose

rose: rose.o module.o debug.o thread.o stack.o ref.o error.o array.o

isa_desc.c: ../doc/isa.dsc
	./isa_gen.pl
	touch $@

isa_tbl.c: ../doc/isa.dsc
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
debug.o: debug.c debug.h ../common/compiler.h
error.o: error.c error.h stack.h debug.h ../common/compiler.h conf.h
isa.o: isa.c thread.h stack.h debug.h ../common/compiler.h conf.h \
 module.h ../common/rmd.h exp.h ptbl.h mtbl.h imp.h cnst.h addr.h \
 text.h sym.h str.h ref.h array.h object.h error.h isa_desc.c \
 isa_tbl.c
isa_desc.o: isa_desc.c
isa_tbl.o: isa_tbl.c
module.o: module.c module.h ../common/rmd.h exp.h ptbl.h mtbl.h imp.h \
 cnst.h addr.h text.h sym.h str.h
ref.o: ref.c ref.h ../common/compiler.h error.h conf.h
rose.o: rose.c module.h ../common/rmd.h exp.h ptbl.h mtbl.h imp.h \
 cnst.h addr.h text.h sym.h str.h thread.h stack.h debug.h \
 ../common/compiler.h conf.h
stack.o: stack.c stack.h debug.h ../common/compiler.h conf.h
thread.o: thread.c thread.h stack.h debug.h ../common/compiler.h \
 conf.h module.h ../common/rmd.h exp.h ptbl.h mtbl.h imp.h cnst.h \
 addr.h text.h sym.h str.h isa.c ref.h array.h object.h error.h \
 isa_desc.c isa_tbl.c
