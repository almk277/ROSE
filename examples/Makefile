.PHONY: all clean

all: hello-1.0.rmd hello-1.1.rmd add.rmd print.rmd

%.rmd: %.ras
	../ras/ras $^ -o $@

clean:
	rm -f *.rmd
