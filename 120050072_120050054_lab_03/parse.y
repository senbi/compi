/* Changes:  */

/* 1. Character constants removed */
/* 2. Changed INTCONSTANT to INT_CONSTANT */
/* 3. Changed the production for constant_expression to include FLOAT_CONSTANT */
/* 4. Added examples of FLOAT_CONSTANTS */
/* 5. Added the description of STRING_LITERAL */
/* 6. Changed primary_expression and FOR */
/* 7. The grammar permits a empty statement. This should be  */
/*    explicitly represented in the AST. */
/* 8. To avoid local decl inside blocks, a rule for statement  */
/*    has been changed. */

/* ----------------------------------------------------------------------- */

/* start symbol is translation_unit */

/* ---------------------------------------------------- */
%scanner Scanner.h
%scanner-token-function d_scanner.lex()

%token VOID INT FLOAT FLOAT_CONSTANT INT_CONSTANT AND_OP OR_OP EQ_OP NE_OP LE_OP GE_OP STRING_LITERAL IF ELSE WHILE FOR RETURN IDENTIFIER INC_OP
%polymorphic unopAttr: otherOP_ast::Kind;stmtAttr:stmt_astnode*;opAttr:Exp_ast*;stmts:std::list<stmt_astnode*>*;exp_stmts:std::list<Exp_ast*>*;strngAttr:std::string; arrAttr:Arrayref_ast*;
%type <unopAttr> unary_operator
%type <stmtAttr> iteration_statement  selection_statement assignment_statement statement compound_statement function_definition
%type <opAttr>  primary_expression  postfix_expression  unary_expression multiplicative_expression additive_expression  relational_expression  equality_expression logical_and_expression expression 
%type <stmts>  statement_list
%type <exp_stmts> expression_list
%type <strngAttr> IDENTIFIER STRING_LITERAL FLOAT_CONSTANT INT_CONSTANT
%type <arrAttr> l_expression
%%

translation_unit
	: function_definition 
	| translation_unit function_definition 
        ;

function_definition
	: type_specifier fun_declarator compound_statement 
	{
		$3->print();
	}
	;

type_specifier
	: VOID 	
    | INT   
	| FLOAT 
    ;

fun_declarator
	: IDENTIFIER '(' parameter_list ')' 
    | IDENTIFIER '(' ')' 
	;

parameter_list
	: parameter_declaration 
	| parameter_list ',' parameter_declaration 
	;

parameter_declaration
	: type_specifier declarator 
        ;

declarator
	: IDENTIFIER 
	| declarator '[' constant_expression ']' 
        ;

constant_expression 
        : INT_CONSTANT 
        | FLOAT_CONSTANT 
        ;

compound_statement
	: '{' '}' 
	{
		$$ =new blockStatement_astnode();
	}
	| '{' statement_list '}' 
	{
		$$ = new blockStatement_astnode($2);
	}
    | '{' declaration_list statement_list '}' 
    {
    	$$ = new blockStatement_astnode($3);
    }
	;

statement_list
	: statement	
	{
		$$ = new std::list<stmt_astnode*>;
		$$->push_back($1);
	}	
    | statement_list statement
    {
    	$$ = $1;
    	$$->push_back($2);
    }	
	;

statement
        : '{' statement_list '}'  //a solution to the local decl problem
        {
        	$$ = new blockStatement_astnode($2);
        }
        | selection_statement 
        {
        	$$ = $1;
        }	
        | iteration_statement
        {
        	$$ = $1;
        } 	
		| assignment_statement
		{
			$$ = $1;
		}	
        | RETURN expression ';'	
        {
        	$$ = new return_astnode($2);
        }
        ;

assignment_statement
	: ';' 					 
	{
		$$ = new ass_astnode();
	}			
	|  l_expression '=' expression ';'	
	{
		$$ = new ass_astnode($1,$3);
	}
	;

expression
        : logical_and_expression 
        {
        	$$ = $1;
        }
        | expression OR_OP logical_and_expression
        {
        	$$ = new OP_ast($1,$3,OP_ast::OR);
        }
	;

logical_and_expression
        : equality_expression
        {
        	$$ = $1;
        }
        | logical_and_expression AND_OP equality_expression 
        {
        	$$ = new OP_ast($1,$3,OP_ast::AND);
        }
	;

equality_expression
	: relational_expression 
	{
		$$ = $1;
	}
    | equality_expression EQ_OP relational_expression 
    {
    	$$ = new OP_ast($1,$3,OP_ast::EQ_OP);
    }	
	| equality_expression NE_OP relational_expression
	{
		$$ = new OP_ast($1,$3,OP_ast::NE_OP);
	}
	;
relational_expression
	: additive_expression
	{
		$$ = $1;
	}
    | relational_expression '<' additive_expression
    {
    	$$ = new OP_ast($1,$3,OP_ast::LT);
    } 
	| relational_expression '>' additive_expression 
	{
		$$ = new OP_ast($1,$3,OP_ast::GT);
	}
	| relational_expression LE_OP additive_expression
	{
		$$ = new OP_ast($1,$3,OP_ast::LE_OP);
	} 
    | relational_expression GE_OP additive_expression 
    {
    	$$ = new OP_ast($1,$3,OP_ast::GE_OP);
    }
	;

additive_expression 
	: multiplicative_expression
	{
		$$ = $1;
	}
	| additive_expression '+' multiplicative_expression 
	{
		$$ = new OP_ast($1,$3,OP_ast::PLUS);
	}
	| additive_expression '-' multiplicative_expression 
	{
		$$ = new OP_ast($1,$3,OP_ast::MINUS);
	}
	;

multiplicative_expression
	: unary_expression
	{
		$$ = $1;
	}
	| multiplicative_expression '*' unary_expression 
	{
		$$ = new OP_ast($1,$3,OP_ast::MULT);
	}
	| multiplicative_expression '/' unary_expression
	{	
		$$ = new OP_ast($1,$3,OP_ast::DIV);
	}
	;
unary_expression
	: postfix_expression
	{
		$$ = $1;
	} 				
	| unary_operator postfix_expression 
	{
		$$ = new otherOP_ast($2,$1);   
	}
	;

postfix_expression
	: primary_expression
	{
		$$ = $1;
	}
    | IDENTIFIER '(' ')'
    {
    	$$ = new funcall_ast($1);
    }
	| IDENTIFIER '(' expression_list ')'
	{
		$$ = new funcall_ast($1,$3);
	} 
	| l_expression INC_OP
	{
		$$ = new otherOP_ast($1,otherOP_ast::PP);
	}
	;

primary_expression
	: l_expression
	{
		$$ = $1;
	}
    | l_expression '=' expression // added this production
    {
    	$$ = new OP_ast($1,$3,OP_ast::ASSIGN);
    }
	| INT_CONSTANT
	{
		$$ = new int_ast(std::atoi(($1).c_str())); 
	}
	| FLOAT_CONSTANT
	{
		$$ = new float_ast(std::atof(($1).c_str())); 
	}
    | STRING_LITERAL
    {
    	$$ = new strconstant_ast($1); 
    }
	| '(' expression ')' 
	{
		$$ = $2;
	}	
	;

l_expression
        : IDENTIFIER
        {
        	$$ = new array_identifier_ast($1); 
        }
        | l_expression '[' expression ']' 
        {
        	$$ = new index_ast($1,$3);
        }	
        ;
expression_list
        : expression
        {	
        	$$ = new std::list<Exp_ast*>;
        	$$->push_back($1);
        }
        | expression_list ',' expression
        {
        	$$ = $1;
        	$$->push_back($3);
        }
        ;
unary_operator
    : '-'
    {
    	$$ = otherOP_ast::UMINUS;
    }
	| '!' 
	{
		$$ = otherOP_ast::NOT;
	}	
	;

selection_statement
        : IF '(' expression ')' statement ELSE statement
        {
        	$$ = new if_astnode($3,$5,$7);
        } 
	;

iteration_statement
	: WHILE '(' expression ')' statement 	
	{
		$$ = new while_astnode($3,$5);
	}
        | FOR '(' expression ';' expression ';' expression ')' statement  //modified this production
        {
        	$$ = new for_astnode($3,$5,$7,$9);
        }
        ;

declaration_list
        : declaration  					
        | declaration_list declaration
	;

declaration
	: type_specifier declarator_list';'
	;

declarator_list
	: declarator
	| declarator_list ',' declarator 
	;


/* A description of integer and float constants. Not part of the grammar.   */

/* Numeric constants are defined as:  */

/* C-constant: */
/*   C-integer-constant */
/*   floating-point-constant */
 
/* C-integer-constant: */
/*   [1-9][0-9]* */
/*   0[bB][01]* */
/*   0[0-7]* */
/*   0[xX][0-9a-fA-F]* */
 
/* floating-point-constant: */
/*   integer-part.[fractional-part ][exponent-part ] */

/* integer-part: */
/*   [0-9]* */
 
/* fractional-part: */
/*   [0-9]* */
 
/* exponent-part: */
/*   [eE][+-][0-9]* */
/*   [eE][0-9]* */

/* The rule given above is not entirely accurate. Correct it on the basis of the following examples: */

/* 1. */
/* 23.1 */
/* 01.456 */
/* 12.e45 */
/* 12.45e12 */
/* 12.45e-12 */
/* 12.45e+12 */

/* The following are not examples of FLOAT_CONSTANTs: */

/* 234 */
/* . */

/* We have not yet defined STRING_LITERALs. For our purpose, these are */
/* sequence of characters enclosed within a pair of ". If the enclosed */
/* sequence contains \ and ", they must be preceded with a \. Apart from */
/* \and ", the only other character that can follow a \ within a string */
/* are t and n.  */
