all:    lex.cc parse.cc main.cc Scanner.h Scannerbase.h Scanner.ih Parser.h Parserbase.h Parser.ih
	g++   --std=c++0x lex.cc parse.cc main.cc;
	./a.out < test-assembly > graph1.gv ;
	sed -i '1s/^/digraph G {\n ordering=out; \n/' graph1.gv ;
	echo "}" >> graph1.gv;  dot -Tps graph1.gv -o graph1.ps;
	evince graph1.ps &
#	egrep 'nextToken|reduce' junk
        

lex.cc: lex.l Scanner.ih 
	./cond_remove_scannerih; 
	flexc++ lex.l; 
	sed -i '/include/a #include "Parserbase.h"' Scanner.ih; 

parse.cc: parse.y Parser.ih Parser.h Parserbase.h
	./cond_remove_parserh;
	bisonc++  parse.y; 
	sed -i '/int parse()/a int val=0;' Parser.h; 
#	bisonc++   --construction -V parse.y; 
#	sed -i '/ifndef/a #include "tree_util.hh"' Parserbase.h;
#	sed -i '/ifndef/a #include "tree.hh"' Parserbase.h;
#	./sedscript
     

Parser.ih: parse.y
Parser.h:  parse.y
Parserbase.h: parse.y
Scanner.ih: lex.l
Scanner.h: lex.l
Scannerbase.h: lex.l

clean :
	rm *.h *.ih lex.cc parse.cc  *~ graph1.gv graph1.ps a.out
