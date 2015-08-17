VOID 					void 
INT 					int 
FLOAT					float 
FUN_DECL	 			\(\)
COMMA			 		\,
SQUARE_BRACKET  		\[|\]
INT_CONSTANT			([0-9][0-9]*)|(0[bB][01]+)|(0[0-7]+)|(0[xX][0-9a-fA-F]+)  
FLOAT_CONSTANT			[0-9]*\.[0-9]*((([eE][\+\-][0-9]*)|([eE][0-9]*))?)
STRING_LITERAL			\"([^\\\n\t\"]*(\\[\\"nt])?)*\"
DIGIT					[0-9]
LETTER					[a-zA-z]
IDENTIFIER				[[:alpha:]_][[:alpha:][:digit:]_]*
BRACES		    		\{|\}
ASSIGN_OP				=
SEMICOLON   			;
OR_OP       			\|\|
AND_OP      			&&
EQ_OP       			==
NOT_OP					!
NEQ_OP      			!=
LE_OP       			\<=
GE_OP       			\>=
GREATERTHAN 			\>
LESSTHAN    			\<
MUL_OP      			\*|\/
INC_OP      			\+\+
ADD_OP					\+|\-
IF		 				if
ELSE	 				else 
WHILE					while 
FOR 					for
WHITESPACE 				[ \t\n]
COMMENT 				\/\*[^\*]*(\*([^\/\*][^\*]*))?*\*\/ 

%%
[ \t]					// skip WHITESPACE characters
[\n]					{line_no++;}
{COMMENT}					// skip COMMENT
{VOID}					{return Parser::VOID;}
{INT}					{return Parser::INT;}
{FLOAT}					{return Parser::FLOAT;}
{IF}					{return Parser::IF;}
{ELSE}					{return Parser::ELSE;}
{WHILE}					{return Parser::WHILE;}
{FOR}					{return Parser::FOR;}
"return"|"RETURN"			{return Parser::RETURN;}
{IDENTIFIER}				{Parser::d_val__ = std::string(matched());return Parser::IDENTIFIER;}
\,						{return ',';}
\[						{return '[';}
\]						{return ']';}
\{						{return '{';}
\}						{return '}';}
\;						{return ';' ;}
\( 						{return '(' ;}
\)						{return  ')' ;}
{OR_OP}					{return Parser::OR_OP;}
{AND_OP}				{return Parser::AND_OP;}
{EQ_OP}					{return Parser::EQ_OP;}
{NEQ_OP}				{return Parser::NE_OP ;}
{LE_OP}					{return Parser::LE_OP;}
{GE_OP}					{return Parser::GE_OP;}
\=						{return '=';}
\<						{return '<';}
\>						{return  '>';}
{INC_OP}				{return Parser::INC_OP;}
\+						{return '+';}
\!						{return  '!';}
\-						{return '-';}
\*						{return '*';}
\/						{return  '/';}
\.						//{return matched()[0];}
{INT_CONSTANT}			{ Parser::d_val__ = std::string(matched()); return Parser::INT_CONSTANT ;}
{FLOAT_CONSTANT}		{Parser::d_val__ = std::string(matched()); return Parser::FLOAT_CONSTANT ;}
{STRING_LITERAL}		{Parser::d_val__ = std::string(matched()); return Parser::STRING_LITERAL;}
.						//{return matched()[0];}

