

%scanner Scanner.h
%scanner-token-function d_scanner.lex()

%token IDENTIFIER VOID INT FLOAT IF ELSE WHILE FOR RETURN NE_OP OR_OP AND_OP 
%token EQ_OP LE_OP GE_OP INC_OP INT_CONSTANT FLOAT_CONSTANT STRING_LITERAL

%%
translation_unit
	: function_definition
	{
	$$=val;

	std::cout<< $$ << "->" << $1 <<";\n";
	std::cout<< $$ << "[label = \"translation_unit\" ];\n";
	std::cout<< $1 << "[label = \"function_definition\" ];\n";
	
	val++; 
	}
	| translation_unit function_definition
	{
	$$=val;
	
 	std::cout<< $$ << "->" << $1 <<";\n";
	std::cout<< $$ << "[label = \"translation_unit\" ];\n";
	std::cout<< $1 << "[label = \"translation_unit\" ];\n";

	std::cout<< $$ << "->" << $2 <<";\n";
	std::cout<< $2 << "[label = \"function_definition\" ];\n";

	val++; 
	}
	;

function_definition
	: type_specifier fun_declarator compound_statement
	{
	$$=val;
	//std::cout<< $$ << "[label = \"function_definition\" ];\n";

	std::cout << $$ << "->" << $1 <<";\n";
	std::cout<< $1 << "[label = \"type_specifier\"];\n";

	std::cout << $$ << "->" << $2 <<";\n";
	std::cout<< $2 << "[label = \"fun_declarator\" ];\n";

	std::cout << $$ << "->" << $3 <<"\n;";
	std::cout<< $3 << "[label = \"compound_statement\" ];\n";
	val++; 
	}
	;

type_specifier
	: VOID
	{ 
	  $$ = val;
	  val=val+1;
	  std::cout << $$ << "->" << val <<";\n";
	  std::cout<< val << "[label = \"VOID\" ];\n";
	  val++;
	}
	| INT
	{ 
	  $$ = val;
	  val=val+1;
	  std::cout << $$ << "->" <<  val <<";\n";
	  std::cout<< val << "[label = \"INT\" ];\n";
	  val++;
	}
	| FLOAT
	{ 
	  $$ = val;
	  val=val+1;
	  std::cout << $$ << "->" <<  val <<";\n";
	 std::cout << val << "[label = \"FLOAT\" ];\n";
	  val++;
	}
	;

fun_declarator
	: IDENTIFIER '(' parameter_list ')'
	{
	$$=val;
 	val=val+1;
	std::cout << $$ << "->" << val <<";\n";
	std::cout<< val << "[label = \"IDENTIFIER\" ];\n";

	val=val+1;
	std::cout<< $$ << "->" << val <<";\n";
	std::cout<< val << "[label = \"(\"];\n";

	std::cout<< $$ << "->" << $3 <<";\n";
	std::cout<< $3 << "[label = \"parameter_list\" ];\n";

	val=val+1;
	std::cout<< $$ << "->" << val <<";\n";
	std::cout<< val << "[label = \")\" ];\n" ;
	val++; 
	}
	| IDENTIFIER '(' ')'
	{
	$$=val;

	val=val+1;
	std::cout << $$ << "->" << val <<";\n";
	std::cout<< val << "[label = \"IDENTIFIER\" ];\n";

	val=val+1;
	std::cout<< $$ << "->" << val <<";\n";
	std::cout<< val << "[label = \"(\" ];\n";

	val=val+1;
	std::cout<< $$ << "->" << val <<";\n";
	std::cout<< val << "[label = \")\" ];\n" ;

	val++; 
	}
	;

parameter_list
	: parameter_declaration
	{
	$$=val;
	std::cout << $$ << "->" << $1 <<";\n";
	std::cout << $1 << "[label = \"parameter_declartion\" ];\n";

	val++; 
	}

	| parameter_list ',' parameter_declaration
	{
	$$=val;

	std::cout << $$ << "->" << $1 <<";\n";
	std::cout<< $1 << "[label = \"parameter_list\" ];\n";

	val=val+1;
	std::cout<< $$ << "->" << val <<";\n";
	std::cout<< val << "[label = \",\" ];\n";

	std::cout<< $$ << "->" << $3 <<";\n";
	std::cout<< $3 << "[label = \"parameter_declartion\" ];\n";
	val++; 
	}
	;

parameter_declaration
	: type_specifier declarator
	{
	$$=val;
	std::cout<< $$ << "->" << $1 <<";\n";
	std::cout<< $1 << "[label = \"type_specifier\" ];\n";

	std::cout<< $$ << "->" << $2 <<";\n";
	std::cout<< $2 << "[label = \"declarator\" ];\n";

	val++; 
	}
	;

declarator
	: IDENTIFIER
	{
	$$=val;
	val=val+1;
	std::cout << $$ << "->" << val <<";\n";
	std::cout<< val << "[label = \"IDENTIFIER\" ];\n";
	val++; 
	}
	| declarator '[' constant_expression ']'
	{
	$$=val;
	std::cout<< $$ << "->" << $1 <<";\n";
	std::cout<< $1 << "[label = \"declarator\" ];\n";
	val=val+1;
	std::cout<< $$ << "->" << val <<";\n";
	std::cout<< val << "[label = \"[\" ];\n";

	std::cout<< $$ << "->" << $3 <<";\n";
	std::cout<< $3 << "[label = \"constant_expression\" ];\n";

	val=val+1;
	std::cout<< $$ << "->" << val <<";\n";
	std::cout<< val << "[label = \"]\" ];\n";
	val++; 
	}
	;

constant_expression
        : INT_CONSTANT
	     {
		$$=val;
		val=val+1;
		std::cout << $$ << "->" << val <<";\n";
		std::cout << val << "[label = \"INT_CONSTANT\" ];\n";
		val++; 
		}
        | FLOAT_CONSTANT
        {
		$$=val;
		val=val+1;
		std::cout << $$ << "->" << val <<";\n";
		std::cout << val << "[label = \"FLOAT_CONSTANT\" ];\n";
		val++; 
		}
        ;

compound_statement
	: '{' '}'
	{
	$$=val;
	val=val+1;
	std::cout << $$ << "->" << val <<";\n";
	std::cout << val << "[label = \"{\" ];\n";
	val=val+1;
	std::cout<< $$ << "->" << val <<";\n";
	std::cout<< val << "[label = \"}\" ];\n" ;
	val++; 
	}
	| '{' statement_list '}'
	{
	$$=val;
	val=val+1;
	std::cout<< $$ << "->" << val <<";\n";
	std::cout << val << "[label = \"{\" ];\n";

	std::cout << $$ << "->" << $2 <<";\n";
	std::cout << $2 << "[label = \"statement_list\" ];\n";

	val=val+1;
	std::cout << $$ << "->" << val <<";\n";
	std::cout << val << "[label = \"}\" ];\n" ;
	val++; 
	}
    | '{' declaration_list statement_list '}'
    {
	$$=val;
	val=val+1;
	std::cout << $$ << "->" << val <<";\n";
	std::cout<< val << "[label = \"{\" ];\n";
	std::cout<< $$ << "->" << $2 <<";\n";
	std::cout<< $2 << "[label = \"declaration_list\" ];\n";
	std::cout<< $$ << "->" << $3 <<";\n";
	std::cout<< $3 << "[label = \"statement_list\" ];\n";
	val=val+1;
	std::cout<< $$ << "->" << val <<";\n";
	std::cout<< val << "[label = \"}\" ];\n" ;
	val++; 
	}
	;

statement_list
	: statement
	{
	$$=val;

	std::cout<< $$ << "->" << $1 <<";\n";
	std::cout<< $1 << "[label = \"statement\" ];\n" ;

	val++; 
	}
	| statement_list statement
	{
	$$=val;

	std::cout<< $$ << "->" << $1 <<";\n";
	std::cout<< $1 << "[label = \"statement_list\" ];\n" ;
	
	std::cout<< $$ << "->" << $2 <<";\n";
	std::cout<< $2 << "[label = \"statement\" ];\n" ;

	val++; 
	}
	;

statement
    : compound_statement
    {
	$$=val;

	std::cout<< $$ << "->" << $1 <<";\n";
	std::cout<< $1 << "[label = \"compound_statement\" ];\n" ;

	val++; 
	}
	| selection_statement
	{
	$$=val;

	std::cout<< $$ << "->" << $1 <<";\n";
	std::cout<< $1 << "[label = \"selection_statement\" ];\n" ;

	val++; 
	}
	| iteration_statement
	{
	$$=val;

	std::cout<< $$ << "->" << $1 <<";\n";
	std::cout<< $1 << "[label = \"iteration_statement\" ];\n" ;

	val++; 
	}
	| assignment_statement
	{
	$$=val;

	std::cout<< $$ << "->" << $1 <<";\n";
	std::cout<< $1 << "[label = \"assignment_statement\" ];\n" ;

	val++; 
	}
    | RETURN expression ';'
    {
	$$=val;

	val=val+1;
	std::cout << $$ << "->" << val <<";\n";
	std::cout << val << "[label = \"RETURN\" ];\n" ;

	std::cout<< $$ << "->" << $2 <<";\n";
	std::cout<< $2 << "[label = \"expression\" ];\n" ;

	val=val+1;
	std::cout << $$ << "->" << val <<";\n";
	std::cout << val << "[label = \";\" ];\n" ;

	val++; 
	}
	;

assignment_statement
	: ';'
	{
	$$=val;

	val=val+1;
	std::cout << $$ << "->" << val <<";\n";
	std::cout << val << "[label = \";\" ];\n" ;

	val++; 
	}
	|  l_expression '=' expression ';'
	{
	$$=val;

	std::cout<< $$ << "->" << $1 <<";\n";
	std::cout<< $1 << "[label = \"l_expression\" ];\n" ;

	val=val+1;
	std::cout << $$ << "->" << val <<";\n";
	std::cout << val << "[label = \"=\" ];\n" ;

	std::cout<< $$ << "->" << $3 <<";\n";
	std::cout<< $3 << "[label = \"expression\" ];\n" ;

	val=val+1;
	std::cout << $$ << "->" << val <<";\n";
	std::cout << val << "[label = \";\" ];\n" ;

	val++; 
	}
	;

expression
	: logical_and_expression
	{
	$$=val;

	std::cout<< $$ << "->" << $1 <<";\n";
	std::cout<< $1 << "[label = \"logical_and_expression\" ];\n" ;

	val++; 
	}
	| expression OR_OP logical_and_expression
	{
	$$=val;

	std::cout<< $$ << "->" << $1 <<";\n";
	std::cout<< $1 << "[label = \"expression OR_OP\" ];\n" ;

	val=val+1;
	std::cout << $$ << "->" << val <<";\n";
	std::cout << val << "[label = \"OR_OP\" ];\n" ;

	std::cout<< $$ << "->" << $3 <<";\n";
	std::cout<< $3 << "[label = \"logical_and_expression\" ];\n" ;

	val++; 
	}
	;

logical_and_expression
	: equality_expression
	{
	$$=val;

	std::cout<< $$ << "->" << $1 <<";\n";
	std::cout<< $1 << "[label = \"equality_expression\" ];\n" ;

	val++; 
	}
	| logical_and_expression AND_OP equality_expression
	{
	$$=val;

	std::cout<< $$ << "->" << $1 <<";\n";
	std::cout<< $1 << "[label = \"logical_and_expression\" ];\n" ;

	val=val+1;
	std::cout << $$ << "->" << val <<";\n";
	std::cout << val << "[label = \"AND_OP\" ];\n" ;

	std::cout<< $$ << "->" << $3 <<";\n";
	std::cout<< $3 << "[label = \"equality_expression\" ];\n" ;
	val++; 
	}
	;

equality_expression
	: relational_expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"relational_expression\"];\n";
		val++;
	}
	| equality_expression EQ_OP relational_expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"equality_expression\"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"EQ_OP\"];\n";

		std::cout<<$$<<"->"<<$3<<";\n";
		std::cout<<$3<<"[label=\"relational_expression\"];\n";

		val++;
	}
	| equality_expression NE_OP relational_expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"equality_expression\"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"NE_OP\"];\n";

		std::cout<<$$<<"->"<<$3<<";\n";
		std::cout<<$3<<"[label=\"relational_expression\"];\n";

		val++;
	}
	;
relational_expression
	: additive_expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"additive_expression\"];\n";

		val++;
	}
	| relational_expression '<' additive_expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"relational_expression\"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"<\"];\n";

		std::cout<<$$<<"->"<<$3<<";\n";
		std::cout<<$3<<"[label=\"additive_expression\"];\n";

		val++;
	}
	| relational_expression '>' additive_expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"relational_expression\"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\">\"];\n";

		std::cout<<$$<<"->"<<$3<<";\n";
		std::cout<<$3<<"[label=\"additive_expression\"];\n";

		val++;
	}
	| relational_expression LE_OP additive_expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"relational_expression\"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"LE_OP\"];\n";

		std::cout<<$$<<"->"<<$3<<";\n";
		std::cout<<$3<<"[label=\"additive_expression\"];\n";

		val++;
	}
	| relational_expression GE_OP additive_expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"relational_expression\"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"GE_OP\"];\n";

		std::cout<<$$<<"->"<<$3<<";\n";
		std::cout<<$3<<"[label=\"additive_expression\"];\n";

		val++;
	}
	;

additive_expression
	: multiplicative_expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"multiplicative_expression\"];\n";

		val++;
	}
	| additive_expression '+' multiplicative_expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"additive_expression\"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"+\"];\n";

		std::cout<<$$<<"->"<<$3<<";\n";
		std::cout<<$3<<"[label=\"multiplicative_expression\"];\n";

		val++;
	}
	| additive_expression '-' multiplicative_expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"additive_expression\"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"-\"];\n";

		std::cout<<$$<<"->"<<$3<<";\n";
		std::cout<<$3<<"[label=\"multiplicative_expression\"];\n";

		val++;
	}
	;

multiplicative_expression
	: unary_expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"unary_expression\"];\n";

		val++;
	}
	| multiplicative_expression '*' unary_expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"multiplicative_expression\"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"*\"];\n";

		std::cout<<$$<<"->"<<$3<<";\n";
		std::cout<<$3<<"[label=\"unary_expression\"];\n";

		val++;
	}
	| multiplicative_expression '/' unary_expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"multiplicative_expression\"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"/\"];\n";

		std::cout<<$$<<"->"<<$3<<";\n";
		std::cout<<$3<<"[label=\"unary_expression\"];\n";

		val++;
	}
	;
unary_expression
	: postfix_expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"postfix_expression\"];\n";

		val++;
	}
	| unary_operator postfix_expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"unary_operator\"];\n";

		std::cout<<$$<<"->"<<$2<<";\n";
		std::cout<<$2<<"[label=\"postfix_expression\"];\n";

		val++;
	}
	;

postfix_expression
	: primary_expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"primary_expression\"];\n";

		val++;
	}
	| IDENTIFIER '(' ')'
	{
		$$ = val;
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"IDENTIFIER\"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"(\"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\")\"];\n";

		val++;
	}
	| IDENTIFIER '(' expression_list ')'
	{
		$$ = val;
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"IDENTIFIER\"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"(\"];\n";

		std::cout<<$$<<"->"<<$3<<";\n";
		std::cout<<$3<<"[label=\"expression_list\"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\")\"];\n";

		val++;
	}
	| l_expression INC_OP
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"l_expression\"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"INC_OP\"];\n";
		val++;
	}
	;

primary_expression
	: l_expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"l_expression\"];\n";

		val++;
	}
	|  l_expression '=' expression // added this production
	{
		$$ = val;
		std::cout << $$ << "->" << $1 <<"\n";
		std::cout<< $1 <<"[label=\"l_expression\"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"=\"];\n";

		std::cout << $$ << "->" << $3 <<"\n";
		std::cout<< $3 <<"[label=\"expression\"];\n";


		val++;
	}
	| INT_CONSTANT
	{
		$$ = val;

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"INT_CONSTANT\"];\n";

		val++;
	}
	| FLOAT_CONSTANT
	{
		$$ = val;

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"FLOAT_CONSTANT\"];\n";

		val++;
	}
        | STRING_LITERAL
	{
		$$ = val;
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"STRING_LITERAL\"];\n";

		val++;
	}
	| '(' expression ')'
	{
		$$ = val;
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"(\"];\n";

		std::cout<<$$<<"->"<<$2<<";\n";
		std::cout<<$2<<"[label=\"expression\"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\")\"];\n";

		val++;
	}
	;

l_expression
        : IDENTIFIER
	{
		$$ = val;
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"IDENTIFIER\"];\n";
		val++;
	}
        | l_expression '[' expression ']' 
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"l_expression\"];\n";
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"[\"];\n";
		std::cout<<$$<<"->"<<$3<<";\n";
		std::cout<<$3<<"[label=\"expression\"];\n";
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"]\"];\n";
		val++;
		
	}
        ;
expression_list
        : expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"expression\"];\n";
		val++;
	}
        | expression_list ',' expression
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"expression_list\"];\n";
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\",\"];\n";
		std::cout<<$$<<"->"<<$3<<";\n";
		std::cout<<$3<<"[label=\"expression\"];\n";
		val++;
	}
        ;
unary_operator
        : '-'
	{
		$$ = val;
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"-\"];\n";
		val++;
	}
	| '!'
	{
		$$ = val;
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"!\"];\n";
		val++;
	}
	;

selection_statement
        : IF '(' expression ')' statement ELSE statement
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"If\"];\n";
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"(\"];\n";
		std::cout<<$$<<"->"<<$3<<";\n";
		std::cout<<$3<<"[label=\"expression\"];\n";
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\")\"];\n";
		std::cout<<$$<<"->"<<$5<<";\n";
		std::cout<<$5<<"[label=\"statement\"];\n";
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"ELSE\"];\n";
		std::cout<<$$<<"->"<<$7<<";\n";
		std::cout<<$5<<"[label=\"statement\"];\n";
		val++;
	}
	;

iteration_statement
	: WHILE '(' expression ')' statement
	{
		$$ = val;
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"WHILE\"];\n";
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"(\"];\n";
		std::cout<<$$<<"->"<<$3<<";\n";
		std::cout<<$3<<"[label=\"expression\"];\n";
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\")\"];\n";
		std::cout<<$$<<"->"<<$5<<";\n";
		std::cout<<$5<<"[label=\"statement\"];\n";
		val++;
	}
	| FOR '(' expression ';' expression ';' expression ')' statement 
	{
		$$ = val;
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"FOR\"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\"(\"];\n";

		std::cout<<$$<<"->"<<$3<<";\n";
		std::cout<<$3<<"[label=\"expression \"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\";\"];\n";

		std::cout<<$$<<"->"<<$5<<";\n";
		std::cout<<$5<<"[label=\"expression \"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\";\"];\n";

		std::cout<<$$<<"->"<<$7<<";\n";
		std::cout<<$7<<"[label=\"expression \"];\n";

		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\")\"];\n";

		std::cout<<$$<<"->"<<$9<<";\n";
		std::cout<<$9<<"[label=\"statement\"];\n";
		val++;
	}
	;

declaration_list
	: declaration
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"declaration\"];\n";
		val++;
	}
	| declaration_list declaration
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"declaration_list\"];\n";
		std::cout<<$$<<"->"<<$2<<";\n";
		std::cout<<$2<<"[label=\"declaration\"];\n";
		val++;
	}
	;

declaration
	: type_specifier declarator_list ';'
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"type_specifier\"];\n";
		std::cout<<$$<<"->"<<$2<<";\n";
		std::cout<<$2<<"[label=\"declarator_list\"];\n";
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\";\"];\n";
		val++;
	}
	;

declarator_list
	: declarator
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"declarator\"];\n";
		val++;
	}
	| declarator_list ',' declarator
	{
		$$ = val;
		std::cout<<$$<<"->"<<$1<<";\n";
		std::cout<<$1<<"[label=\"declaration_list\"];\n";
		std::cout<<$$<<"->"<<++val<<";\n";
		std::cout<<val<<"[label=\",\"];\n";
		std::cout<<$$<<"->"<<$3<<";\n";
		std::cout<<$3<<"[label=\"declarator\"];\n";
		val++;
	}
	;
