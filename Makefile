.PHONY: all clean examples

all: ras/ras rose/rose

ras/ras:
	cd ras && $(MAKE)

rose/rose:
	cd rose && $(MAKE)

examples:
	cd examples && $(MAKE)

clean:
	cd ras && $(MAKE) clean
	cd rose && $(MAKE) clean
	cd examples && $(MAKE) clean

