#include <iostream>
#include <string>
#include <list>
seq_astnode::seq_astnode(){
	stmt_0 = NULL;
	stmt_1 = NULL;
}
seq_astnode::seq_astnode(stmt_astnode* a,stmt_astnode*b){
	stmt_0 = a;
	stmt_1 = b;
}
void seq_astnode::print(){
	std::cout<<"(SEQ ";
	stmt_0->print() ;
	std::cout<<"\n";
	stmt_1->print();
	std::cout<<" )\n";
}
blockStatement_astnode::blockStatement_astnode(){
	l = NULL;
}
blockStatement_astnode::blockStatement_astnode(std::list<stmt_astnode*>*a){
	l = a;
}
void blockStatement_astnode::insert(stmt_astnode* stmt){
	l->push_back(stmt);
}
void blockStatement_astnode::print(){
	if(l == NULL){
		std::cout << "(Block [empty] )";
		return;
	}
	std::cout << "(Block [";
	std::list<stmt_astnode*>::iterator it;
	for(it = l->begin() ; it != l->end() ; ++it){
		(*it)->print();
	}
	std::cout<<" ])\n";
}
ass_astnode::ass_astnode(){
	exp_0 = NULL;
	exp_1 = NULL;
}
ass_astnode::ass_astnode(Exp_ast* a ,Exp_ast* b){
	if (a->t->tag == Type::Error ||  // error propagation
		      b->t->tag == Type::Error)
	   	{t = new Type(Type::Error);}
	 	else {
	 	t=result(a->t, b->t);
	 }

		if(t->tag == Type::Error){exp_0=a;exp_1=b;}
		else if (a->t->basetype == Type::Int && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				exp_0=a;exp_1=b;
			}else {exp_0=a;exp_1=new ToInt_ast(b);}
		}
		else if(a->t->basetype == Type::Float && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				exp_0=a;exp_1=new ToFloat_ast(b);
			}else {exp_0=a;exp_1=b;}
		}else {exp_0=a;exp_1=b;}
	//exp_0 = a;
	//exp_1 = b;
}
void ass_astnode::print(){
	if(exp_0 == NULL)std::cout<<"(Empty)";
	else{
		std::cout<<"(Assign ";
		exp_0->print();
		exp_1->print();
		std::cout<<" )\n";
	}
}
return_astnode::return_astnode(){
	exp_0 = NULL;
}
return_astnode::return_astnode(Exp_ast* a){
	exp_0 = a;
}
void return_astnode::print(){
	std::cout<<"(Return ";
	exp_0->print();
	std::cout<<" )\n";
}
funstmt_ast::funstmt_ast(){
	args = NULL;
}
funstmt_ast::funstmt_ast(std::string s){
	identifier_ast* idast = new identifier_ast(s);
	args->push_back(idast);
}
funstmt_ast::funstmt_ast(std::string s,std::list<Exp_ast*>* a){
	args = a;
	identifier_ast* idast = new identifier_ast(s);
	args->push_front(idast);
}
void funstmt_ast::insert(Exp_ast* a){
	args->push_back(a);
}
void funstmt_ast::print(){
	std::cout << "(funstmt ";
	std::list<Exp_ast*>::iterator it;
	it=args->begin();
	(*it)->print();
	std::cout << "(ARGS  ";
	++it;
	for(; it != args->end() ; ++it){
		(*it)->print();
	}
	std::cout<<") )\n";
}
if_astnode::if_astnode(){
	exp_0 = NULL;
	stmt_1 = NULL;
	stmt_2 =NULL;
}
if_astnode::if_astnode(Exp_ast* a,stmt_astnode* b,stmt_astnode* c){
	exp_0 = a;
	stmt_1 = b;
	stmt_2 = c;
}
void if_astnode::print(){
	std::cout<<"(If ";
	exp_0->print();
	stmt_1->print();
	stmt_2->print();
	std::cout<<" )\n";
}
while_astnode::while_astnode(){
	exp_0 = NULL;
	stmt_1 =NULL;
}
while_astnode::while_astnode(Exp_ast* a,stmt_astnode* b){
	exp_0 = a;
	stmt_1 = b;
}
void while_astnode::print(){
	std::cout<<"(While ";
	exp_0->print();
	stmt_1->print();
	std::cout<<" )\n";
}
for_astnode::for_astnode(){
	exp_0 = NULL;
	exp_1 = NULL;
	exp_2 = NULL;
	stmt_3 =NULL;
}
for_astnode::for_astnode(Exp_ast* a,Exp_ast* b,Exp_ast* c,stmt_astnode* d){
	
	if((a->t->tag == Type::Error || b->t->tag == Type::Error) || (c->t->tag == Type::Error
        	 || d->t->tag == Type::Error) ){
        		t == new Type(Type::Error);
        		if (t->tag == Type::Error)
			    {std::cout << "Error \n"; print(); /*ABORT();*/}
        	}
        else t == new Type(Type::Ok);
	exp_0 = a;
	exp_1 = b;
	exp_2 = c;
	stmt_3 = d;
}
void for_astnode::print(){
	std::cout<<"(For ";
	exp_0->print();
	exp_1->print();
	exp_2->print();
	/*if (stmt_3==NULL)
	{
		std::cout << "(empty)";
	}
	else*/
	 stmt_3->print();
	std::cout<<" )\n";
}
OP_ast::OP_ast(){
	lhs = NULL;
	rhs = NULL;
}
OP_ast::OP_ast(Exp_ast* a,Exp_ast* b,Kind c){
	if (a->t->tag == Type::Error ||  // error propagation
		     b->t->tag == Type::Error)
		   	{t = new Type(Type::Error);
		   	std::cout << "Error \n"; print(); /*ABORT();*/}
		 	else {
		 	t=result(a->t, b->t);
		 	if (t->tag == Type::Error)
			{ std::cout << "Error \n"; print(); /*ABORT();*/}
			}
	if(c == PLUS){
		if(t->tag == Type::Error){op =c; lhs = a; rhs = b;}
		else if (a->t->basetype == Type::Int && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = PLUS;
				lhs = a;
				rhs = b;
			}else {op = F_PLUS; lhs = a; rhs = new ToFloat_ast(b);}
		}
		else if(a->t->basetype == Type::Float && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = F_PLUS;
				lhs = new ToFloat_ast(a);
				rhs = b;
			}else {op = F_PLUS; lhs = a; rhs = b;}
		}else {op =c; lhs = a; rhs = b;}
	}else if(c == MINUS){
		if(t->tag == Type::Error){op =c; lhs = a; rhs = b;}
		else if (a->t->basetype == Type::Int && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = MINUS;
				lhs = a;
				rhs = b;
			}else {op = F_MINUS; lhs = a; rhs = new ToFloat_ast(b);}
		}
		else if(a->t->basetype == Type::Float && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = F_MINUS;
				lhs = new ToFloat_ast(a);
				rhs = b;
			}else {op = F_MINUS; lhs = a; rhs = b;}
		}else {op =c; lhs = a; rhs = b;}
	}else if(c == LT){
		if(t->tag == Type::Error){op =c; lhs = a; rhs = b;}
		else if (a->t->basetype == Type::Int && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = LT;
				lhs = a;
				rhs = b;
			}else {op = F_LT; lhs = a; rhs = new ToFloat_ast(b);}
		}
		else if(a->t->basetype == Type::Float && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = F_LT;
				lhs = new ToFloat_ast(a);
				rhs = b;
			}else {op = F_LT; lhs = a; rhs = b;}
		}else {op =c; lhs = a; rhs = b;}
	}
	else if(c == GT){
		if(t->tag == Type::Error){op =c; lhs = a; rhs = b;}
		else if (a->t->basetype == Type::Int && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = GT;
				lhs = a;
				rhs = b;
			}else {op = F_GT; lhs = a; rhs = new ToFloat_ast(b);}
		}
		else if(a->t->basetype == Type::Float && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = F_GT;
				lhs = new ToFloat_ast(a);
				rhs = b;
			}else {op = F_GT; lhs = a; rhs = b;}
		}else {op =c; lhs = a; rhs = b;}
	}else if(c == LE_OP){
		if(t->tag == Type::Error){op =c; lhs = a; rhs = b;}
		else if (a->t->basetype == Type::Int && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = LE_OP;
				lhs = a;
				rhs = b;
			}else {op = F_LE_OP; lhs = a; rhs = new ToFloat_ast(b);}
		}
		else if(a->t->basetype == Type::Float && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = F_LE_OP;
				lhs = new ToFloat_ast(a);
				rhs = b;
			}else {op = F_LE_OP; lhs = a; rhs = b;}
		}else {op =c; lhs = a; rhs = b;}
	}else if(c == GE_OP){
		if(t->tag == Type::Error){op =c; lhs = a; rhs = b;}
		else if (a->t->basetype == Type::Int && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = GE_OP;
				lhs = a;
				rhs = b;
			}else {op = F_GE_OP; lhs = a; rhs = new ToFloat_ast(b);}
		}
		else if(a->t->basetype == Type::Float && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = F_GE_OP;
				lhs = new ToFloat_ast(a);
				rhs = b;
			}else {op = F_GE_OP; lhs = a; rhs = b;}
		}else {op =c; lhs = a; rhs = b;}
	}else if(c == MULT){
		if(t->tag == Type::Error){op =c; lhs = a; rhs = b;}
		else if (a->t->basetype == Type::Int && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = MULT;
				lhs = a;
				rhs = b;
			}else {op = F_MULT; lhs = a; rhs = new ToFloat_ast(b);}
		}
		else if(a->t->basetype == Type::Float && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = F_MULT;
				lhs = new ToFloat_ast(a);
				rhs = b;
			}else {op = F_MULT; lhs = a; rhs = b;}
		}else {op =c; lhs = a; rhs = b;}
	}else if(c == DIV){
		if(t->tag == Type::Error){op =c; lhs = a; rhs = b;}
		else if (a->t->basetype == Type::Int && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = DIV;
				lhs = a;
				rhs = b;
			}else {op = F_DIV; lhs = a; rhs = new ToFloat_ast(b);}
		}
		else if(a->t->basetype == Type::Float && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = F_DIV;
				lhs = new ToFloat_ast(a);
				rhs = b;
			}else {op = F_DIV; lhs = a; rhs = b;}
		}else {op =c; lhs = a; rhs = b;}
	}
	else if (c == ASSIGN){
		op = c ;
		if(t->tag == Type::Error){lhs=a;rhs=b;}
		else if (a->t->basetype == Type::Int && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				lhs=a;rhs=b;
			}else {lhs=a;rhs=new ToInt_ast(b);}
		}
		else if(a->t->basetype == Type::Float && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				lhs=a;rhs=new ToFloat_ast(b);
			}else {lhs=a;rhs=b;}
		}else {lhs=a;rhs=b;}
	}
	else {op =c; lhs = a; rhs = b;}
}
void OP_ast::print(){
	std::cout<<"(";
	switch(op){
		case OR : 
			std::cout<<"OR ";
			lhs->print();
			rhs->print();
			break;
		case AND : 
			std::cout<<"AND ";
			lhs->print();
			rhs->print();
			break;
		case EQ_OP : 
			std::cout<<"EQ ";
			lhs->print();
			rhs->print();
			break;
		case NE_OP : 
			std::cout<<"NE ";
			lhs->print();
			rhs->print();
			break;
		case GT : 
			std::cout<<"GT_Int ";
			lhs->print();
			rhs->print();
			break;
		case F_GT :
			std::cout<<"GT_Float ";
			lhs->print();
			rhs->print();
			break;
		case LT : 
			std::cout<<"LT_Int ";
			lhs->print();
			rhs->print();
			break;
		case F_LT :
			std::cout<<"LT_Float ";
			lhs->print();
			rhs->print();
			break;
		case LE_OP : 
			std::cout<<"LE_int ";
			lhs->print();
			rhs->print();
			break;
		case F_LE_OP :
			std::cout<<"LE_Float ";
			lhs->print();
			rhs->print();
			break;
		case GE_OP : 
			std::cout<<"GE_Int ";
			lhs->print();
			rhs->print();
			break;
		case F_GE_OP :
			std::cout<<"GE_Float ";
			lhs->print();
			rhs->print();
			break;
		case PLUS : 
			std::cout<<"Plus_Int ";
			lhs->print();
			rhs->print();
			break;
		case F_PLUS :
			std::cout<<"Plus_Float ";
			lhs->print();
			rhs->print();
			break;
		case MINUS : 
			std::cout<<"Minus_Int ";
			lhs->print();
			rhs->print();
			break;
		case F_MINUS :
			std::cout<<"Minus_Float ";
			lhs->print();
			rhs->print();
			break;
		case MULT : 
			std::cout<<"Mult_Int ";
			lhs->print();
			rhs->print();
			break;
		case F_MULT :
			std::cout<<"Mult_Float ";
			lhs->print();
			rhs->print();
			break;
		case DIV : 
			std::cout<<"Div_Int ";
			lhs->print();
			rhs->print();
			break;
		case F_DIV :
			std::cout<<"Div_Float ";
			lhs->print();
			rhs->print();
			break;
		default :
			std::cout<<"Assign_exp ";
			lhs->print();
			rhs->print();
	}
	std::cout<<" )\n";
}

otherOP_ast::otherOP_ast(){
	exp_0 = NULL;
}
otherOP_ast::otherOP_ast(Exp_ast* a,Kind b){
	exp_0 = a;
	op = b;
}
void otherOP_ast::print(){
	std::cout<<"(";
	switch(op){
	case UMINUS : 
		std::cout<<"Minus ";
		break;
	case NOT : 
		std::cout<<"Not ";
		break;
	case PP : 
		std::cout<<"PP ";
		break;
	}
	exp_0->print();
	std::cout<<" )\n";
}
funcall_ast::funcall_ast(){
	args = NULL;
}
funcall_ast::funcall_ast(std::string s){
	identifier_ast* idast = new identifier_ast(s);
	args->push_back(idast);
}
funcall_ast::funcall_ast(std::string s,std::list<Exp_ast*>* a){
	args = a;
	identifier_ast* idast = new identifier_ast(s);
	args->push_front(idast);
}
void funcall_ast::insert(Exp_ast* a){
	args->push_back(a);
}
void funcall_ast::print(){
	std::cout << "(Funcall ";
	std::list<Exp_ast*>::iterator it;
	it=args->begin();
	(*it)->print();
	std::cout << "(ARGS  ";
	++it;
	for(; it != args->end() ; ++it){
		(*it)->print();
	}
	std::cout<<") )\n";
}
float_ast::float_ast(float a){
	value = a;
}
void float_ast::print(){
	std::cout<<"(FLOATconst "<<value<<" )";
}
int_ast::int_ast(int a){
	value = a;
}
void int_ast::print(){
	std::cout<<"(INTconst "<<value<<" )";
}
strconstant_ast::strconstant_ast(std::string st){
	s= st;
}
void strconstant_ast::print(){
	std::cout<<"(StrConst "<<s<<" )";
}
identifier_ast::identifier_ast(std::string s){
	id =s;
}
void identifier_ast::print(){
	std::cout<<"(Identifier "<<id<<" )";
} 
array_identifier_ast::array_identifier_ast(std::string s){
	id =s;
}
void array_identifier_ast::print(){
	std::cout<<"(Identifier "<<id<<" )";
}
index_ast::index_ast(){
	ref= NULL;
	ex = NULL;
}
index_ast::index_ast(Arrayref_ast* a,Exp_ast* b){
	ref = a;
	ex = b;
}
void index_ast::print(){
	std::cout<<"(Index ";
	ref->print();
	ex->print();
	std::cout<<" )\n";
}
ToFloat_ast::ToFloat_ast(){
	exp_0 = NULL;
	t= new Type (Type::Base , Type::Float);
}
ToFloat_ast::ToFloat_ast(Exp_ast* a){
	exp_0 = a;
	t= new Type (Type::Base , Type::Float);
}
void ToFloat_ast::print(){
	std::cout<<" ( To_Float ";
	exp_0->print();
	std::cout<<" ) ";
}
ToInt_ast::ToInt_ast(){
	exp_0 = NULL;
	t= new Type (Type::Base , Type::Int);
}
ToInt_ast::ToInt_ast(Exp_ast* a){
	exp_0 = a;
	t= new Type (Type::Base , Type::Int);
}
void ToInt_ast::print(){
	std::cout<<" ( To_Int ";
	exp_0->print();
	std::cout<<" ) ";
}