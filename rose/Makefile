.PHONY		: all clean debug

CXXFLAGS += -Wall

all			: rose text2int tags

rose		: main.o debug.o stack.o isa.o text.o data.o\
				module.o opcode.o file.o instr.o segment.o thread.o
	$(CXX) $(LDFLAGS) -o $@ $^

ras			: ras.o

text2int	: text2int.o

clean		:
	$(RM) *.o rose ras text2int

debug		: CFLAGS += -g
debug		: CPPFLAGS += -g
debug		: LDFLAGS += -g
debug		: rose

tags		: *.cpp
	ctags *.cpp
