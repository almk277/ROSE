.PHONY: all clean clean-all depend

CFLAGS += -Wall -g
CPPFLAGS += -I../include

all: rose

rose: rose.o module.o debug.o thread.o stack.o text.o \
 ptbl.o exp.o sym.o cnst.o addr.o ref.o str.o error.o array.o

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
addr.o: addr.c addr.h ../include/rmd.h
array.o: array.c array.h object.h error.h ref.h
cnst.o: cnst.c cnst.h ../include/rmd.h
debug.o: debug.c debug.h compiler.h
error.o: error.c error.h stack.h debug.h compiler.h conf.h
exp.o: exp.c exp.h ../include/rmd.h
isa.o: isa.c thread.h stack.h debug.h compiler.h conf.h module.h \
 ../include/rmd.h exp.h ptbl.h cnst.h addr.h text.h sym.h str.h ref.h \
 array.h object.h error.h isa_desc.c isa_tbl.c
isa_desc.o: isa_desc.c
isa_tbl.o: isa_tbl.c
module.o: module.c module.h ../include/rmd.h exp.h ptbl.h cnst.h \
 addr.h text.h sym.h str.h
ptbl.o: ptbl.c ptbl.h ../include/rmd.h
ref.o: ref.c ref.h compiler.h error.h conf.h
rose.o: rose.c module.h ../include/rmd.h exp.h ptbl.h cnst.h addr.h \
 text.h sym.h str.h thread.h stack.h debug.h compiler.h conf.h
stack.o: stack.c stack.h debug.h compiler.h conf.h
str.o: str.c str.h ../include/rmd.h
sym.o: sym.c sym.h ../include/rmd.h
text.o: text.c text.h ../include/rmd.h
thread.o: thread.c thread.h stack.h debug.h compiler.h conf.h module.h \
 ../include/rmd.h exp.h ptbl.h cnst.h addr.h text.h sym.h str.h isa.c \
 ref.h array.h object.h error.h isa_desc.c isa_tbl.c
