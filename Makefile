.PHONY: all clean sources

all: ras/ras rose/rose

ras/ras:
	cd ras && $(MAKE)

rose/rose:
	cd rose && $(MAKE)

sources:
	cd sources && $(MAKE)

clean:
	cd ras && $(MAKE) clean
	cd rose && $(MAKE) clean
	cd sources && $(MAKE) clean

