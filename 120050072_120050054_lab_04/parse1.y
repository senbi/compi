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
//%debug
%scanner Scanner.h
%scanner-token-function d_scanner.lex()

%token VOID INT FLOAT FLOAT_CONSTANT INT_CONSTANT AND_OP OR_OP EQ_OP NE_OP LE_OP GE_OP STRING_LITERAL IF ELSE WHILE FOR RETURN IDENTIFIER INC_OP
%polymorphic unopAttr: otherOP_ast::Kind;stmtAttr:stmt_astnode*;opAttr:Exp_ast*;stmts:std::list<stmt_astnode*>*;exp_stmts:std::list<Exp_ast*>*;strngAttr:std::string;cpar:check_par*; arrAttr:Arrayref_ast*;funcvar:func_name*;typefunc:func_tn*;dAttr:dec_ast*;cast:const_ast*;
%type <unopAttr> unary_operator
%type <stmtAttr> iteration_statement  selection_statement assignment_statement statement compound_statement function_definition
%type <opAttr>  primary_expression  postfix_expression  unary_expression multiplicative_expression additive_expression  relational_expression  equality_expression logical_and_expression expression 
%type <stmts>  statement_list
%type <exp_stmts> expression_list
%type <strngAttr> IDENTIFIER STRING_LITERAL FLOAT_CONSTANT INT_CONSTANT type_specifier  
%type <arrAttr> l_expression
%type <funcvar>  fun_declarator
%type <dAttr> declarator
%type <cast> constant_expression
%%

translation_unit
	: function_definition 
	| translation_unit function_definition 
        ;

function_definition
	: type_specifier 
	  fun_declarator
	  compound_statement 
		{	
			std::cout << "Function :" << current_func << "\n";
			STable* temper = globalST.lookup($2->fname);
			temper->print();
			std::cout << "\n";
			$3->print();
			std::cout << "\n";
		}
	;

type_specifier
	: VOID 	
	{
		$$ = "VOID";
	}
    | INT  
    {
    	$$ = "INT";
    } 
	| FLOAT 
	{
		$$ = "FLOAT";
	}
    ;

fun_declarator
	: IDENTIFIER 
	'(' 
	{
		current_func = $1;
		globalST.insert($1,new STable($<strngAttr>0));
		$<funcvar>$ = new func_name($1);
	}
	parameter_list 

	')' 
	{
		$$ = $3;
	}
    | IDENTIFIER '(' ')' 
    {
    	current_func = $1;
    	globalST.insert($1,new STable($<strngAttr>0));
    	$<funcvar>$ = new func_name($1);
    }
	;

parameter_list
	: parameter_declaration 
	| parameter_list ','
	{
		$$ = $<funcvar>0;
	} parameter_declaration 
	;

parameter_declaration
	: type_specifier {
		if ($1=="VOID")
		{
			std::cout << "Error : in line "<< line_no << " : "; // having void parameter in function declaration
			std::cout << "VOID type is not to be declared\n";
			 ABORT();
		}
		else 
		{
			$<typefunc>$ = new func_tn($<funcvar>0,$1,true);
		}

	}
	declarator {
		STable* t = globalST.lookup($<typefunc>2->f->fname);
		if (t->lookup($3->vname)==NULL)
		{

			t->insert_args($3->vname,new VarInfo($3->t,t->offset,$3->length));
			t->offset += $3->length;
		}
		else 
		{

			std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "Duplicate declaration of variable\n";
			 ABORT();
		}
		
	}
    ;

declarator
	: IDENTIFIER 
	{
		//STable* t = globalST.lookup($<typefunc>0->f->fname);
		//if($<typefunc>0->args)
		//t->insert_args($1,new VarInfo($<typefunc>0->t,$<typefunc>0->update(t->offset)));
		//else
		//t->insert($1,new VarInfo($<typefunc>0->t,$<typefunc>0->update(t->offset)));
		//std::cout<<$1<<" "<<$<typefunc>0->offset<<" enterd\n";
		$$ = new dec_ast($1,$<typefunc>0->t,$<typefunc>0->offset);
	}
	| declarator '[' constant_expression ']' {
		//STable* t = globalST.lookup($<typefunc>0->f->fname);
		//Type* temp = new Type(Type::Array,$<typefunc>0->t);
		//$<typefunc>0->t = temp;
		//if($<typefunc>0->args)
		//t->insert_args("pranay",new VarInfo($<typefunc>0->t,$<typefunc>0->update(t->offset)));
		//else
		//t->insert("pranay",new VarInfo($<typefunc>0->t,$<typefunc>0->update(t->offset)));
		if($3->t->basetype == Type::Float)
		{
			std::cout << "Error : in line "<< line_no << " : "; // assignment error 
			std::cout << "Array index must be an Int\n";
			 ABORT();
		}
		else{
			
			$$ = new dec_ast($1 , $3->value);
		}
		
	}
	;


constant_expression 
        : INT_CONSTANT 
        {
        	$$ = new const_ast(new Type(Type::Base,Type::Int),int(std::atoi(($1).c_str())));
        	//$$->t = new Type(Type::Base,Type::Int);
        }
        | FLOAT_CONSTANT 
        {
        	$$ = new const_ast(new Type(Type::Base,Type::Float),float(std::atof(($1).c_str())));
        	//$$->t = new Type(Type::Base,Type::Float);
        }
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
    | '{'
    {
    	$<funcvar>$ = $<funcvar>0;
    } 
    declaration_list statement_list '}' 
    {
    	$$ = new blockStatement_astnode($4);
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
        	$$->t = new Type(Type::Ok);
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
        	$$->t = $2->t;
        }
        | IDENTIFIER '(' ')' ';'
	    {

	    	STable *temp  = globalST.lookup($1);

	    	
	    	if(temp == NULL)
	    	{
	    		//$$->t = new Type(Type::Error);    		
	        	std::cout << "Error : in line "<< line_no << " : "; 
				std::cout << "function with such a name doesnt exist\n";
				ABORT();
				
	    	}
	    	else 
	    	{
	    	$$ = new funstmt_ast($1);
	    	$$->t = temp->returntype;
	    	}
	    }
		| IDENTIFIER '(' 
		{	STable *temp  = globalST.lookup($1);
			if (temp==NULL)
			{
				$$->t = new Type(Type::Error);    		
	        	std::cout << "Error : in line "<< line_no << " : "; 
				std::cout << "function with such a name doesnt exist\n";
				ABORT();
			}
			else 
			{
				$<cpar>$ = new check_par(temp->args); 
			}
		}
		expression_list ')' ';'
		{
			if ($<cpar>3->args.size()>$<cpar>3->argno)
			{
				std::cout << "Error : in line "<< line_no << " : "; 
				std::cout << "Error : More function arguments are expected\n";
				 ABORT();
			}
			STable *temp  = globalST.lookup($1);
			$$ = new funstmt_ast($1,$4);
			if(temp == NULL){
	    		$$->t = new Type(Type::Error);
	    		std::cout << "Error \n"; $$->print(); ABORT();
	    	}else {
	    	$$->t = temp->returntype;}
		} 
        ;

assignment_statement
	: ';' 					 
	{
		$$ = new ass_astnode();
		$$->t = new Type(Type::Ok);
	}			
	|  l_expression '=' expression ';'	
	{
		$$ = new ass_astnode($1,$3);
		
	 	if ($$->t->tag == Type::Error)
	 	 {	std::cout << "Error : in line "<< line_no << " : "; // assignment error 
			std::cout << "Error in assignment\n";
			 ABORT();
			 }
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
        	if ($$->t->tag == Type::Error) 
        	{
        	std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "Or of the two expressions is not viable\n";
			 ABORT();
			 }
        	else 
        	{
        	$$->t = new Type(Type::Base,Type::Int);
        	}
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
    	 	if ($$->t->tag == Type::Error) 
    	 	{
        	std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "AND of the two expressions is not viable\n";
			 ABORT();
			 }
    	 	else 
    	 	{
    	 	$$->t = new Type(Type::Base,Type::Int);
    	 	}
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
    	if ($$->t->tag == Type::Error) 
    	{
        	std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "EQ_OP of the two expressions is not viable\n";
			 ABORT();
			 }
    }	
	| equality_expression NE_OP relational_expression
	{
		$$ = new OP_ast($1,$3,OP_ast::NE_OP);
		if ($$->t->tag == Type::Error) 
		{
        	std::cout << "Error : in line "<< line_no << " : ";
			std::cout << "Not Equal of the two expressions is not viable\n";
			 ABORT();
		}
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
    	if ($$->t->tag == Type::Error) 
    	{
        	std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "Less Than of the two expressions is not viable\n";
			 ABORT();
			 }
    } 
	| relational_expression '>' additive_expression 
	{
		$$ = new OP_ast($1,$3,OP_ast::GT);
		if ($$->t->tag == Type::Error) 
		{
        	std::cout << "Error : in line "<< line_no << " : ";
			std::cout << "Greater Than of the two expressions is not viable\n";
			 ABORT();
			 }
	}
	| relational_expression LE_OP additive_expression
	{
		$$ = new OP_ast($1,$3,OP_ast::LE_OP);
		if ($$->t->tag == Type::Error) 
		{
        	std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "Less Than or Equal to of the two expressions is not viable\n";
			 ABORT();
			 }
	} 
    | relational_expression GE_OP additive_expression 
    {
    	$$ = new OP_ast($1,$3,OP_ast::GE_OP);
    	if ($$->t->tag == Type::Error) 
    	{
        	std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "Greater Than or equal to of the two expressions is not viable\n";
			 ABORT();
			 }
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

		if ($$->t->tag == Type::Error) 
		{
        	std::cout << "Error : in line "<< line_no << " : ";
			std::cout << "Addition of the two expressions is not viable\n";
			 ABORT();
		}
		
		

		
	}
	| additive_expression '-' multiplicative_expression 
	{
		$$ = new OP_ast($1,$3,OP_ast::MINUS);
		if ($$->t->tag == Type::Error) 
		{
        	std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "Subtraction of the two expressions is not viable\n";
			 ABORT();
			 }
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
		if ($$->t->tag == Type::Error) 
		{
        	std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "Multiplication of the two expressions is not viable\n";
			 ABORT();
			 }
	}
	| multiplicative_expression '/' unary_expression
	{	
		$$ = new OP_ast($1,$3,OP_ast::DIV);
		if ($$->t->tag == Type::Error) 
		{
        	std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "Division of the two expressions is not viable\n";
			 ABORT();
			 }
	}
	;
unary_expression
	: postfix_expression
	{
		$$ = $1;
	} 				
	| unary_operator postfix_expression 
	{
		if ($2->t->tag == Type::Array)
		{
			std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "can't Apply the unary operator \n";
			ABORT();
		}
		Type* temp ;
		if ($1==otherOP_ast::UMINUS)
		{
			temp = $2->t ;
		}
		else if ($1==otherOP_ast::NOT)
		{
			temp = new Type(Type::Base,Type::Int);
		}
		$$ = new otherOP_ast($2,$1);   // not implemented
		$$->t = temp;
	}
	;

postfix_expression
	: primary_expression
	{
		$$ = $1;

	}
    | IDENTIFIER '(' ')'
    {

    	STable *temp  = globalST.lookup($1);

    	
    	if(temp == NULL)
    	{
    		//$$->t = new Type(Type::Error);    		
        	std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "function with such a name doesnt exist\n";
			ABORT();
			
    	}
    	else 
    	{
    	$$ = new funcall_ast($1);
    	$$->t = temp->returntype;
    	}
    }
	| IDENTIFIER '(' 
	{	STable *temp  = globalST.lookup($1);
		if (temp==NULL)
		{
			$$->t = new Type(Type::Error);    		
        	std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "function with such a name doesnt exist\n";
			ABORT();
		}
		else 
		{
			$<cpar>$ = new check_par(temp->args); 
		}
	}
	expression_list ')'
	{
		if ($<cpar>3->args.size()>$<cpar>3->argno)
		{
			std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "Error : More function arguments are expected\n";
			 ABORT();
		}
		STable *temp  = globalST.lookup($1);
		$$ = new funcall_ast($1,$4);
		if(temp == NULL){
    		$$->t = new Type(Type::Error);
    		std::cout << "Error \n"; $$->print(); ABORT();
    	}else {
    	$$->t = temp->returntype;}
	} 
	| l_expression INC_OP
	{
		Type*t=$1->t;
		if(t->tag == Type::Array)
		{
			std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "Increment operator not supported for array\n";
		}
		$$ = new otherOP_ast($1,otherOP_ast::PP); 
		$$->t=t;
		if($$->t->tag == Type::Error) 
		{
			std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "Error in applying increment operator\n";
			ABORT();
		}
	}
	;

primary_expression
	: l_expression
	{
		$$ = $1;
		//$$->t->printType();

	}
    | l_expression '=' expression // added this production
    {
    	$$ = new OP_ast($1,$3,OP_ast::ASSIGN);
    	if ($$->t->tag == Type::Error)
		  {std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "Assignment error\n";
			ABORT();}
    }
	| INT_CONSTANT
	{
		$$ = new int_ast(std::atoi(($1).c_str())); 
		$$->t = new Type(Type::Base,Type::Int);
	}
	| FLOAT_CONSTANT
	{
		$$ = new float_ast(std::atof(($1).c_str())); 
		$$->t = new Type(Type::Base,Type::Float);
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
        	STable *temp  = globalST.lookup(current_func); 
        	if (temp->lookup($1)==NULL ) {
        	$$->t=new Type(Type::Error);
        	std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "no such array as : "<< $1 << "\n";
			ABORT();
			}
        	else {
			 	$$->t = (temp->lookup($1))->t;
			 	

			 	}
        }
        | l_expression '[' expression ']' 
        {
        	
        	Arrayref_ast* temp = $1;
        	$$ = new index_ast($1,$3);
        	if(temp->t->tag == Type::Error){
        		$$->t = temp->t;
			    std::cout << "Error : in line "<< line_no << " : "; 
				std::cout << "Error in using array\n";
			 	ABORT();
		  
        	}
        	else if(temp->t->tag == Type::Base){
        		$$->t = new Type(Type::Error);
			    std::cout << "Error : in line "<< line_no << " : "; 
				std::cout << "Error Array\n";
			 	ABORT();
        	}else{
        		if ($3->t->basetype != Type::Int)
        		{
        			std::cout << "Error : in line "<< line_no << " : "; 
					std::cout << "Array index should be integer\n";
			 		ABORT();
        		} 
        		$$->t = temp->t->pointed;

        	}
        }	
        ;
expression_list
        : expression
        {	Type*temp = $<cpar>0->args[$<cpar>0->argno].second->t ;
        	if ($1->t->tag==Type::Error)
        	{
        		std::cout << "Error : in line "<< line_no << " : "; 
				std::cout << "Error in a parameter\n";
			 	ABORT();
        	}
        	else 
        	{
        		Type* temp1 = result ($1->t,temp);
        		if (temp1->tag == Type::Error )
        		{
        			std::cout << "Error : in line "<< line_no << " : "; 
					std::cout << "Non Compatible Parameters are passed \n";
			 		ABORT();
        		} 
        		$<cpar>0->argno++;
        		$$ = new std::list<Exp_ast*>;
        		$$->push_back($1);
        	}
        	
        }
        | expression_list ','
         expression
        {
        	if ($<cpar>0->args.size()<=$<cpar>0->argno)
        	{
        		std::cout << "Error : in line "<< line_no << " : "; 
				std::cout << "Too many arguments in function\n";
			 ABORT();
        	}
        	Type*temp = $<cpar>0->args[$<cpar>0->argno].second->t ;
        	//std::cout << $<cpar>0->args.size() << " " << $<cpar>0->argno << "\n";
        	if ($3->t->tag==Type::Error)
        	{
        		std::cout << "Error : in line "<< line_no << " : "; 
				std::cout << "Error in parameter\n";
			 	ABORT();
        	}
        	else 
        	{
        		Type* temp1 = result ($3->t,temp);
        		//$3->t->printType();
        		//std::cout << "  ";
        		//temp->printType();
        		//std::cout << "\n";
        		if (temp1->tag == Type::Error )
        		{
        			std::cout << "Error : in line "<< line_no << " : "; 
					std::cout << "Non Compatible parameters \n";
			 			ABORT();
        		} 
        		$<cpar>0->argno++;
        		$$ = $1;
        	$$->push_back($3);
        	}
        	
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
        	if($3->t->tag == Type::Error || $5->t->tag == Type::Error || $7->t->tag == Type::Error){
        		std::cout << "Error : in line "<< line_no << " : "; 
				std::cout << "error in \n";
			 	ABORT();
        		$$->t == new Type(Type::Error);
        		if ($$->t->tag == Type::Error)
			    {
			    std::cout << "Error : in line "<< line_no << " : "; 
				std::cout << "If expression is not compatible\n";
			 	ABORT();
			 }
        	}else $$->t == new Type(Type::Ok);
        } 
	;

iteration_statement
	: WHILE '(' expression ')' statement 	
	{
		$$ = new while_astnode($3,$5);
		if($3->t->tag == Type::Error || $5->t->tag == Type::Error ){
				std::cout << "Error : in line "<< line_no << " : "; 
				std::cout << "Error in \n";
			 	ABORT();
        		$$->t == new Type(Type::Error);
        		if ($$->t->tag == Type::Error)
			    {
			    std::cout << "Error : in line "<< line_no << " : "; 
				std::cout << "While expresssion is not compatible\n";
			 	ABORT();
			 }
        	}else $$->t == new Type(Type::Ok);
	}
        | FOR '(' expression ';' expression ';' expression ')' statement  //modified this production
        {
        	$$ = new for_astnode($3,$5,$7,$9);
        	if($3->t->tag == Type::Error || $5->t->tag == Type::Error || $7->t->tag == Type::Error || $9->t->tag == Type::Error ){
				std::cout << "Error : in line "<< line_no << " : "; 
				std::cout << "Error in \n";
			 	ABORT();
        		$$->t == new Type(Type::Error);
        		if ($$->t->tag == Type::Error)
			    {
			    std::cout << "Error : in line "<< line_no << " : "; 
				std::cout << "While expresssion is not compatible\n";
			 	ABORT();
			 }
        	}else $$->t == new Type(Type::Ok);
        	
        	
        }
        ;

declaration_list
        : declaration  					
        | declaration_list {
        	$<funcvar>$ = $<funcvar>0;
        } declaration
	;

declaration
	: type_specifier
	{	if ($1 == "VOID"){
			std::cout << "Error : in line "<< line_no << " : "; // having void parameter in function declaration
			std::cout << "VOID type is not to be declared\n";
			 ABORT();
		}

		$<typefunc>$ = new func_tn($<funcvar>0,$1,false);
	} declarator_list';'
	;

declarator_list
	: declarator 
	{
		STable* t = globalST.lookup($<typefunc>0->f->fname);
		if (t->lookup($1->vname)==NULL)
		{

			t->insert($1->vname,new VarInfo($1->t,t->offset,$1->length));
		t->offset += $1->length;
		}
		else 
		{
			
			std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "Duplicate declaration of variable\n";
			 ABORT();
		}
		
	}
	| declarator_list ',' 
	{
		$<typefunc>$ = $<typefunc>0;
	}declarator {
		STable* t = globalST.lookup($<typefunc>3->f->fname);
		// if variable not declared before then insert it 
		if (t->lookup($4->vname)==NULL)
		{

			t->insert($4->vname,new VarInfo($4->t,t->offset,$4->length));
			t->offset += $4->length;
		}
		else 
		{

			std::cout << "Error : in line "<< line_no << " : "; 
			std::cout << "Duplicate declaration of variable\n";
			 ABORT();
		}
		
	}
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
