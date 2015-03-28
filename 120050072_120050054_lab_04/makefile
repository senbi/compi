all:    lex.cc parse.cc main.cc 
	./sedscript
	g++   --std=c++11 lex.cc parse.cc main.cc;
	./a.out < test-assembly > junk;

run:    a.out
	./a.out < test-assembly >junk;
lex.cc: lex.l  Scanner.ih
	flexc++ lex.l; 
#	sed -i '/include/a #include "Parserbase.h"' Scanner.ih; 

parse.cc: parse.y Parser.ih 
	bisonc++ --construction  parse1.y; 
#	bisonc++   --construction -V parse.y; 
#	sed -i '/insert preincludes/a #include "headers.cc"' Parserbase.h;
#	sed -i '/include "Parser.h"/a #include "definitions.cc"' Parser.ih;

.PHONY: clean
clean:
	rm -f Scanner*
	rm -f Parser*
	rm -f parse.cc parse.y.output lex.cc a.out junk 
Parser.ih: parse.y
Parser.h:  parse.y
Parserbase.h: parse.y
Scanner.ih: lex.l
Scanner.h: lex.l
Scannerbase.h: lex.l
