.PHONY: all clean examples

all: lib/librose.a ras/ras rose/rose

lib/librose.a:
	cd lib && $(MAKE)

ras/ras:
	cd ras && $(MAKE)

rose/rose:
	cd rose && $(MAKE)

examples:
	cd examples && $(MAKE)

clean:
	cd lib && $(MAKE) clean
	cd ras && $(MAKE) clean
	cd rose && $(MAKE) clean
	cd examples && $(MAKE) clean

