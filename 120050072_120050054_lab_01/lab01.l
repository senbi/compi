VOID 					void 
INT 					int 
FLOAT					float 
FUN_DECL	 			\(\)
COMMA			 		\,
SQUARE_BRACKET  		\[|\]
integer-part			[0-9]*
fractional-part			[0-9]*	
exponent-part			([eE][+-][0-9]*|[eE][0-9]*)
FLOATTWO				([0-9]{integer-part}\.{fractional-part}?{exponent-part}?)|(\.[0-9]{fractional-part}{exponent-part}?)
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
RETURN 					RETURN
WHITESPACE 				[ \t\n]
COMMENT 				\/\*[^\*]*(\*([^\/\*][^\*]*))?*\*\/ 

%%
[ \t\n]					// skip WHITESPACE characters
{COMMENT}					// skip COMMENT
{VOID}					{return VOID;}
{INT}					{return INT;}
{FLOAT}					{return FLOAT;}
{IF}					{return IF;}
{ELSE}					{return ELSE;}
{WHILE}					{return WHILE;}
{FOR}					{return FOR;}
{RETURN}				{return RETURN;}
{IDENTIFIER}				{return IDENTIFIER;}
\,						{return ',';}
\[						{return '[';}
\]						{return ']';}
\{						{return '{';}
\}						{return '}';}
\;						{return ';' ;}
\( 						{return '(' ;}
\)						{return  ')' ;}
{OR_OP}					{return OR_OP;}
{AND_OP}				{return AND_OP;}
{EQ_OP}					{return EQ_OP;}
{NEQ_OP}				{return NEQ_OP ;}
{LE_OP}					{return LE_OP;}
{GE_OP}					{return GE_OP;}
\=						{return '=';}
\<						{return '<';}
\>						{return  '>';}
{INC_OP}				{return INC_OP;}
\+						{return '+';}
\!						{return  '!';}
\-						{return '-';}
\*						{return '*';}
\/						{return  '/';}
\.						{return matched()[0];}
{INT_CONSTANT}			{return INT_CONSTANT ;}
{FLOAT_CONSTANT}		{return FLOAT_CONSTANT ;}
{STRING_LITERAL}		{return STRING_LITERAL;}
.						{return matched()[0];}

  
