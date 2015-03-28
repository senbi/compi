#include <iostream>
#include "Scanner.h"
#include "Parser.h"
using namespace std;
int main (int argc, char** arg)
{
  Parser parser;
  parser.parse();
  STable* endcheck = globalST.lookup("main");
  if (endcheck==NULL)
  {
  	std::cout << "Error : Function main not found \n";
  }
}


