.PHONY: all clean

all: ras

ras:
	cd ras && $(MAKE) ras

clean:
	cd ras && $(MAKE) clean

