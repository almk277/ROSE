.PHONY: all clean

all: hello.rmd print.rmd

%.rmd: %.ras
	../ras/ras $^ -o $@

clean:
	rm -f *.rmd
