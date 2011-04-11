all: rmdutil

CPPFLAGS += -I../include
CFLAGS += -g -Wall

rmdutil: main.o
	cc $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o rmdutil
