all:compfs.o statSem.o parser.o scanner.o main.o scanner.h token.h parser.h compfs.h
	g++ main.o compfs.o parser.o scanner.o statSem.o -o compfs

%.o:%.cpp scanner.h token.h parser.h statSem.h compfs.h
	g++ -c $@ $<

.PHONY : all clean

clean: 
	@rm -f *.o compfs *.asm