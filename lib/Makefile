all: librose.a

librose.a: symtbl.o mm.o loader.o cmdopt.o
	$(AR) crs $@ $^

debug:   CPPFLAGS += -DDEBUG
debug:   CFLAGS += -Wall -g
debug:   librose.a

clean:
	rm -f *.o librose.a

tags: *.c
	ctags *.c
