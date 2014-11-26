.PHONY: all clean

all: hello.rmd print.rmd answer.rmd

%.rmd: %.ras
	../ras/ras $^ -output=$@

clean:
	rm -f *.rmd
