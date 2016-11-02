.PHONY: all clean examples

all: lib/librose.a rasm/rasm rose/rose

lib/librose.a:
	cd lib && $(MAKE)

rasm/rasm:
	cd rasm && $(MAKE)

rose/rose:
	cd rose && $(MAKE)

examples:
	cd examples && $(MAKE)

clean:
	cd lib && $(MAKE) clean
	cd rasm && $(MAKE) clean
	cd rose && $(MAKE) clean
	cd examples && $(MAKE) clean

