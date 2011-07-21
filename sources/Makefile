.PHONY: all clean

all: hello.rmd add.rmd

%.rmd: %.ras
	../ras/ras $^ -o $@

clean:
	rm -f *.rmd
