.PHONY: all clean clean-all depend

CFLAGS += -Wall -g
LDFLAGS += -g
CPPFLAGS += -I../include
LIBS += -L../lib -lrose

all: rose

rose: rose.o module.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

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
