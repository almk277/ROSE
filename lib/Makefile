CPPFLAGS += -I../include

all: librose.a

librose.a: symtbl.o mm.o symbol.o loader.o
	$(AR) crs $@ $^

debug:   CPPFLAGS += -DDEBUG
debug:   CFLAGS += -Wall -g
debug:   librose.a

clean:
	rm -f *.o librose.a

tags: *.c
	ctags *.c
