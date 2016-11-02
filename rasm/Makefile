CXX := clang++
CPPFLAGS := -I ../lib
CXXFLAGS := -Wall -std=c++14 -g -O2
LDFLAGS := -g -O2

all: rasm

rasm: rasm.o scanner.o module.o instr.o parser.o ../lib/cmdopt.o
	$(CXX) $(LDFLAGS) -o $@ $^

instr.o: instr.cpp
instr.cpp: gen ../doc/isa.dsc ../doc/operand.dsc
	./gen 

gen: gen.c

clean:
	$(RM) *.o rasm

clean-all: clean
	$(RM) instr.cpp gen

dep:
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM *.cpp

instr.o: instr.cpp instr.hpp
module.o: module.cpp module.hpp serial.hpp ../lib/rmd.h
parser.o: parser.cpp parser.hpp scanner.hpp instr.hpp module.hpp \
  serial.hpp ../lib/rmd.h
rasm.o: rasm.cpp scanner.hpp parser.hpp module.hpp serial.hpp \
  ../lib/rmd.h ../lib/cmdopt.h
scanner.o: scanner.cpp scanner.hpp
