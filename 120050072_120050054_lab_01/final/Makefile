all:    lex.cc  main.cc 
	g++   --std=c++0x lex.cc main.cc

lex.cc: lab01.l  
	cond_remove_scannerh
	flexc++ lab01.l; 
	sed -i '/int lex()/r enums_2' Scanner.h



