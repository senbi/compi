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
	  cout << "identifier: " << text << '\n';
	  break;
	case Scanner::VOID:
	  cout<<"void: "<<text<<'\n';
	  break;
	case	Scanner::INT:
	  cout<<"int: "<<text<<'\n';
	  break;
	case	Scanner::FLOAT:	
	  cout<<"float: "<<text<<'\n';
	  break;
	case	Scanner::IF:
	  cout<<"if :"<<text<<'\n';
	  break;
	case 	Scanner::ELSE:	
	  cout<<"else :"<<text<<'\n';
	  break;
	case 	Scanner::FOR:	
	  cout<<"for :"<<text<<'\n';
	  break;
	case	Scanner::WHILE:
	  cout<<"while :"<<text<<'\n';
	  break;
	case 	Scanner::RETURN:
	  cout<<"Return :"<<text<<'\n';
	  break;
	case	Scanner::OR_OP:
	  cout<<"Or operator: "<<text<<'\n';
	  break;
	case	Scanner::AND_OP:
	  cout<<"And operator: "<<text<<'\n';
	  break;
	case	Scanner::EQ_OP:
	  cout<<"Equal operator: "<<text<<'\n';
	  break;
	case	Scanner::NEQ_OP:
	  cout<<"Not Equals operator: "<<text<<'\n';
	  break;
	case	Scanner::LE_OP:
	  cout<<"Less than or Equal to : "<<text<<'\n';
	  break;
	case	Scanner::GE_OP:
	  cout<<"Greater than or Equal to: "<<text<<'\n';
	  break;
	case	Scanner::INC_OP:
	  cout<<"Increament Operator: "<<text<<'\n';
	  break;
	case 	Scanner::INT_CONSTANT:
	  cout<<"Integer Constant: "<<text<<'\n';
	  break;
	case	Scanner::FLOAT_CONSTANT:
	  cout<<"Float Constant: "<<text<<'\n';
	  break;
	case	Scanner::STRING_LITERAL:
	  cout<<"String Literal: "<<text<<'\n';
	  break;
	case	'=':
		cout <<"assign operator: " <<text<<'\n';
	  break;
	case	'<':
		cout <<"less than: " <<text<<'\n';
	  break;
	case	'>':
		cout <<"greater than: " <<text<<'\n';
	  break;
	case	'!':
		cout <<" not : " <<text<<'\n';
	  break;
	case	'+':
		cout <<" addition: " <<text<<'\n';
	  break;
	case	'-':
		cout <<" subtraction: " <<text<<'\n';
	  break;
	case	'*':
		cout <<"multiplication: " <<text<<'\n';
	  break;
	case	'/':
		cout <<"division : " <<text<<'\n';
	  break;
	case	'(':
		cout <<" ( bracket: " <<text<<'\n';
	  break;
	case	')':
		cout <<" ) bracket: " <<text<<'\n';
	  break;
	case	'[':
		cout <<" [ bracket: " <<text<<'\n';
	  break;
	case	']':
		cout <<" ] bracket: " <<text<<'\n';
	  break;
	case	'{':
		cout <<" { bracket: " <<text<<'\n';
	  break;
	case	'}':
		cout <<" } bracket: " <<text<<'\n';
	  break;
	case	',':
		cout <<"coma : " <<text<<'\n';
	  break;
	case	';':
		cout <<" semi colon: " <<text<<'\n';
	  break;
	
	default:
	  cout << "char. token: `" << text << "'\n";
	}
    }
}
