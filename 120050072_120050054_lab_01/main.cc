#include <iostream>
#include "Scanner.h"
using namespace std;
int main()
{
  Scanner scanner;   // define a Scanner object
  while (int token = scanner.lex()) // get all tokens
    {
      string const &text = scanner.matched();
      switch (token)
	{
	case Scanner::IDENTIFIER:
	  cout << "IDENTIFIER: " << text << '\n';
	  break;
	case Scanner::VOID:
	  cout<<"VOID: "<<text<<'\n';
	  break;
	case	Scanner::INT:
	  cout<<"Integer: "<<text<<'\n';
	  break;
	case	Scanner::FLOAT:	
	  cout<<"FLOAT: "<<text<<'\n';
	  break;
	case	Scanner::IF:
	  cout<<"IF:"<<text<<'\n';
	  break;
	case 	Scanner::ELSE:	
	  cout<<"ELSE:"<<text<<'\n';
	  break;
	case 	Scanner::FOR:	
	  cout<<"FOR:"<<text<<'\n';
	  break;
	case	Scanner::WHILE:
	  cout<<"WHILE:"<<text<<'\n';
	  break;
	case 	Scanner::RETURN:
	  cout<<"RETURN:"<<text<<'\n';
	  break;
	case	Scanner::OR_OP:
	  cout<<"OR_OP: "<<text<<'\n';
	  break;
	case	Scanner::AND_OP:
	  cout<<"AND_OP: "<<text<<'\n';
	  break;
	case	Scanner::EQ_OP:
	  cout<<"EQ_OP: "<<text<<'\n';
	  break;
	case	Scanner::NEQ_OP:
	  cout<<"NEQ_OP: "<<text<<'\n';
	  break;
	case	Scanner::LE_OP:
	  cout<<"LE_OP: "<<text<<'\n';
	  break;
	case	Scanner::GE_OP:
	  cout<<"GE_OP: "<<text<<'\n';
	  break;
	case	Scanner::INC_OP:
	  cout<<"INC_OP: "<<text<<'\n';
	  break;
	case 	Scanner::INT_CONSTANT:
	  cout<<"INT_CONSTANT: "<<text<<'\n';
	  break;
	case	Scanner::FLOAT_CONSTANT:
	  cout<<"FLOAT_CONSTANT: "<<text<<'\n';
	  break;
	case	Scanner::STRING_LITERAL:
	  cout<<"STRING_LITERAL: "<<text<<'\n';
	  break;
	case	'=':
		cout <<"assign_operator: " <<text<<'\n';
	  break;
	case	'<':
		cout <<"less_than: " <<text<<'\n';
	  break;
	case	'>':
		cout <<"greater_than: " <<text<<'\n';
	  break;
	case	'!':
		cout <<"not: " <<text<<'\n';
	  break;
	case	'+':
		cout <<"addition: " <<text<<'\n';
	  break;
	case	'-':
		cout <<"subtraction: " <<text<<'\n';
	  break;
	case	'*':
		cout <<"multiplication: " <<text<<'\n';
	  break;
	case	'/':
		cout <<"division : " <<text<<'\n';
	  break;
	case	'(':
		cout <<"( bracket: " <<text<<'\n';
	  break;
	case	')':
		cout <<") bracket: " <<text<<'\n';
	  break;
	case	'[':
		cout <<"[ bracket: " <<text<<'\n';
	  break;
	case	']':
		cout <<"] bracket: " <<text<<'\n';
	  break;
	case	'{':
		cout <<"{ bracket: " <<text<<'\n';
	  break;
	case	'}':
		cout <<"} bracket: " <<text<<'\n';
	  break;
	case	',':
		cout <<"coma : " <<text<<'\n';
	  break;
	case	';':
		cout <<"semi colon: " <<text<<'\n';
	  break;
	
	default:
	  cout << "char. token: `" << text << "'\n";
	}
    }
}
