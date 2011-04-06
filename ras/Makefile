all: ras
DESCR = ../doc/isa.dsc

CFLAGS += -Wall -g

ras: main.o common.o dirmap.o direct.o str.o mm.o hash.o section.o tables.o \
		storage.o instr.o proc.o text.o
	$(CC) $(LDFLAGS) -o $@ $^

dirmap.c: dirmap.gperf
	gperf -LANSI-C --output-file $@ < $^

instr.gperf: $(DESCR) instr-head.gperf
	(cat instr-head.gperf; perl isa2perf.pl < $<) > $@

instr.c: instr.gperf
	gperf -LANSI-C --output-file $@ < $^

clean:
	rm -f *.o ras

clean-all: clean
	rm -f dirmap.c instr.c instr.gperf
