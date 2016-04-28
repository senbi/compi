#include <iostream>
#include <string>
#include <list>
#define I 4
#define F 4
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
void seq_astnode::generate_code(){
	//TODO
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
void blockStatement_astnode::generate_code(){
	std::list<stmt_astnode*>::iterator it,it1;
	std::string s1;
	it = l->begin();
	(*it)->generate_code();
	it1 = it;
	++it;
	for(;it!=l->end();it1=it,++it){
		if((*it1)->nextlist.size() != 0){
			s1 = newLabel();
			genCode(s1+":");
			backpatch((*it1)->nextlist,s1);
		}
		(*it)->generate_code();
	}
	--it;
	nextlist = (*it)->nextlist;
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
void ass_astnode::generate_code(){
	if(exp_0 == NULL)return;
	exp_1->generate_code();
 	exp_0->generate_code();
 	if(t->basetype == Type::Int){
		genCode(indent+"loadi(ind(esp,4),eax);");
		genCode(indent+"storei(eax,ind(ebp,edx));");
		genCode(indent+"popi(1);");
		genCode(indent+"popi(1);");
	}else if(t->basetype == Type::Float){
		genCode(indent+"loadf(ind(esp,4),eax);");
		genCode(indent+"storef(eax,ind(ebp,edx));");
		genCode(indent+"popf(1);");
		genCode(indent+"popf(1);");
	}
}
return_astnode::return_astnode(){
	exp_0 = NULL;
}
return_astnode::return_astnode(Exp_ast* a){
	STable* temp = globalST.lookup(current_func);
	Type* returntype = temp->returntype;
	if (a->t->tag == Type::Error)
   	{t = new Type(Type::Error);}
 	else {
 		t=result(a->t,returntype);
	}
	if(t->tag != Type::Error){
		if (a->t->basetype == Type::Int && returntype->basetype == Type::Float){
			exp_0 = new ToFloat_ast(a);
		}
		else if(a->t->basetype == Type::Float && returntype->basetype == Type::Int){
				exp_0 = new ToInt_ast(a);
		}else exp_0 = a;
	}else exp_0 = a;
}
void return_astnode::print(){
	std::cout<<"(Return ";
	exp_0->print();
	std::cout<<" )\n";
}
void return_astnode::generate_code(){
	STable *temp  = globalST.lookup(current_func);
	if(temp->returntype->basetype == Type::Int){
		exp_0->generate_code();
		genCode(indent+"loadi(ind(esp),eax);");
		genCode(indent+"popi(1);");
		genCode(indent+"storei(eax,ind(ebp,"+std::to_string((temp->args.size()+1)*4)+"));");
		genCode(indent+ "j(e);");
	}else if(temp->returntype->basetype == Type::Float){
		exp_0->generate_code();
		genCode(indent+"loadf(ind(esp),eax);");
		genCode(indent+"popf(1);");
		genCode(indent+"storef(eax,ind(ebp,"+std::to_string((temp->args.size()+1)*4)+"));");
		genCode(indent+ "j(e);");
	}

}
funstmt_ast::funstmt_ast(){
	args = NULL;
}
funstmt_ast::funstmt_ast(std::string s){
	func_name =s;
	identifier_ast* idast = new identifier_ast(s);
	args = new std::list<Exp_ast*>;
	args->push_back(idast);
}
funstmt_ast::funstmt_ast(std::string s,std::list<Exp_ast*>* a){
	func_name =s;
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
void funstmt_ast::generate_code(){
	Type* temp = (globalST.lookup(func_name))->returntype;
	std::list<Exp_ast*>::iterator it;
	it = args->begin();
	++it;
	if(temp->basetype == Type::Int){
		genCode(indent+"pushi(0); //for storing return value");
	}else if(temp->basetype == Type::Float){
		genCode(indent+"pushf(0); //for storing return value");
	}
	for(;it!=args->end();++it){
		(*it)->generate_code();
	}
	genCode(indent+func_name+"();");
	--it;
	for(;it!=args->begin();--it){
		if((*it)->t->basetype == Type::Int){
			genCode(indent+"popi(1);");
		}else if((*it)->t->basetype == Type::Float){
			genCode(indent+"popf(1);");
		} 
	}
	if(temp->basetype == Type::Int){
		genCode(indent+"popi(1);");
	}else if(temp->basetype == Type::Float){
		genCode(indent+"popf(1);");
	}

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
void if_astnode::generate_code(){
	std::string t1 = newLabel();
	exp_0->generate_code();
	if(exp_0->t->basetype == Type::Int){
		genCode(indent+"loadi(ind(esp, 0), eax);");
		genCode(indent+"popi(1);");
		genCode(indent+"cmpi(0,eax);");
		genCode(indent+"je("+t1+");");
		// code for true case 
		stmt_1->generate_code();
		genCode(indent+"j");
		nextlist.push_back(gCodeArray.size());
		merge(nextlist,stmt_1->nextlist);
		genCode(t1+":");
		stmt_2->generate_code();
		merge(nextlist,stmt_2->nextlist);
	}else{
		genCode(indent+"loadf(ind(esp, 0), eax);");
		genCode(indent+"popf(1);");
		genCode(indent+"cmpf(0,eax);");
		genCode(indent+"je("+t1+");");
		// code for true case 
		stmt_1->generate_code();
		genCode(indent+"j");
		nextlist.push_back(gCodeArray.size());
		merge(nextlist,stmt_1->nextlist);
		genCode(t1+":");
		stmt_2->generate_code();
		merge(nextlist,stmt_2->nextlist);
	}
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
void while_astnode::generate_code(){
	std::string t1 = newLabel();
	genCode(t1+":");
	exp_0->generate_code();
	if(exp_0->t->basetype == Type::Int){
		genCode(indent+"loadi(ind(esp, 0), eax);");
		genCode(indent+"popi(1);");
		genCode(indent+"cmpi(0,eax);");
		genCode(indent+"je");
		nextlist.push_back(gCodeArray.size());
		stmt_1->generate_code();
		backpatch(stmt_1->nextlist,t1);
		genCode(indent+"j("+t1+");");
	}else {
		genCode(indent+"loadf(ind(esp, 0), eax);");
		genCode(indent+"popf(1);");
		genCode(indent+"cmpf(0,eax);");
		genCode(indent+"je");
		nextlist.push_back(gCodeArray.size());
		stmt_1->generate_code();
		backpatch(stmt_1->nextlist,t1);
		genCode(indent+"j("+t1+");");
	}
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
			    {std::cout << "Error \n";  /*ABORT();*/}
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
	stmt_3->print();
	std::cout<<" )\n";
}
void for_astnode::generate_code(){
	std::string t1 = newLabel(),t2;
	exp_0->generate_code();
	if(exp_0->t->basetype==Type::Int )
	{
		genCode(indent+"popi(1);");
	}
	else if(exp_0->t->basetype==Type::Float )
	{
		genCode(indent+"popf(1);");
	}
	genCode(t1+":");
	exp_1->generate_code();
	if(exp_1->t->basetype == Type::Int){
		genCode(indent+"loadi(ind(esp, 0), eax);");
		genCode(indent+"popi(1);");
		genCode(indent+"cmpi(0,eax);");
		genCode(indent+"je");
		nextlist.push_back(gCodeArray.size());
		stmt_3->generate_code();
		if(stmt_3->nextlist.size()!=0){
			t2=newLabel();
			genCode(t2+":");
			backpatch(stmt_3->nextlist,t2);
		}
		exp_2->generate_code();
		if(exp_0->t->basetype==Type::Int )
		{
			genCode(indent+"popi(1);");
		}
		else if(exp_0->t->basetype==Type::Float )
		{
			genCode(indent+"popf(1);");
		}
		genCode(indent+"j("+t1+");");
		//nextlist.push_back(gCodeArray.size());
	}else {
		genCode(indent+"loadf(ind(esp, 0), eax);");
		genCode(indent+"popi(1);");
		genCode(indent+"cmpf(0,eax);");
		genCode(indent+"je");
		nextlist.push_back(gCodeArray.size());
		stmt_3->generate_code();
		if(stmt_3->nextlist.size()!=0){
			t2=newLabel();
			genCode(t2+":");
			backpatch(stmt_3->nextlist,t2);
		}
		exp_2->generate_code();
		if(exp_0->t->basetype==Type::Int )
		{
			genCode(indent+"popi(1);");
		}
		else if(exp_0->t->basetype==Type::Float )
		{
			genCode(indent+"popf(1);");
		}
		genCode(indent+"j("+t1+");");
		//nextlist.push_back(gCodeArray.size());
	}
}
OP_ast::OP_ast(){
	lhs = NULL;
	rhs = NULL;
}
OP_ast::OP_ast(Exp_ast* a,Exp_ast* b,Kind c){
	if (a->t->tag == Type::Error ||  // error propagation
		     b->t->tag == Type::Error)
		   	{t = new Type(Type::Error);
		   	std::cout << "Error \n";  /*ABORT();*/}
		 	else {
		 	t=result(a->t, b->t);
		 	if (t->tag == Type::Error)
			{ std::cout << "Error \n";  /*ABORT();*/}
			}
	if(c == PLUS){
		if(t->tag == Type::Error){op =c; lhs = a; rhs = b;}
		else if (a->t->basetype == Type::Int && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = PLUS;
				lhs = a;
				rhs = b;
			}else {op = F_PLUS; lhs = new ToFloat_ast(a); rhs = b;}
		}
		else if(a->t->basetype == Type::Float && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = F_PLUS;
				lhs = a;
				rhs = new ToFloat_ast(b);
			}else {op = F_PLUS; lhs = a; rhs = b;}
		}else {op =c; lhs = a; rhs = b;}
	}else if(c == MINUS){
		if(t->tag == Type::Error){op =c; lhs = a; rhs = b;}
		else if (a->t->basetype == Type::Int && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = MINUS;
				lhs = a;
				rhs = b;
			}else {op = F_MINUS; lhs = new ToFloat_ast(a); rhs = b;}
		}
		else if(a->t->basetype == Type::Float && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = F_MINUS;
				lhs = a;
				rhs = new ToFloat_ast(b);
			}else {op = F_MINUS; lhs = a; rhs = b;}
		}else {op =c; lhs = a; rhs = b;}
	}else if(c == LT){
		if(t->tag == Type::Error){op =c; lhs = a; rhs = b;}
		else if (a->t->basetype == Type::Int && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = LT;
				lhs = a;
				rhs = b;
			}else {op = F_LT; lhs = new ToFloat_ast(a); rhs = b;}
		}
		else if(a->t->basetype == Type::Float && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = F_LT;
				lhs = a;
				rhs = new ToFloat_ast(b);
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
			}else {op = F_GT; lhs = new ToFloat_ast(a); rhs = b;}
		}
		else if(a->t->basetype == Type::Float && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = F_GT;
				lhs = a;
				rhs = new ToFloat_ast(b);
			}else {op = F_GT; lhs = a; rhs = b;}
		}else {op =c; lhs = a; rhs = b;}
	}else if(c == LE_OP){
		if(t->tag == Type::Error){op =c; lhs = a; rhs = b;}
		else if (a->t->basetype == Type::Int && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = LE_OP;
				lhs = a;
				rhs = b;
			}else {op = F_LE_OP; lhs = new ToFloat_ast(a); rhs = b;}
		}
		else if(a->t->basetype == Type::Float && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = F_LE_OP;
				lhs = a;
				rhs = new ToFloat_ast(b);
			}else {op = F_LE_OP; lhs = a; rhs = b;}
		}else {op =c; lhs = a; rhs = b;}
	}else if(c == GE_OP){
		if(t->tag == Type::Error){op =c; lhs = a; rhs = b;}
		else if (a->t->basetype == Type::Int && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = GE_OP;
				lhs = a;
				rhs = b;
			}else {op = F_GE_OP; lhs = new ToFloat_ast(a); rhs = b;}
		}
		else if(a->t->basetype == Type::Float && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = F_GE_OP;
				lhs = a;
				rhs = new ToFloat_ast(b);
			}else {op = F_GE_OP; lhs = a; rhs = b;}
		}else {op =c; lhs = a; rhs = b;}
	}else if(c == MULT){
		if(t->tag == Type::Error){op =c; lhs = a; rhs = b;}
		else if (a->t->basetype == Type::Int && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = MULT;
				lhs = a;
				rhs = b;
			}else {op = F_MULT; lhs = new ToFloat_ast(a); rhs = b;}
		}
		else if(a->t->basetype == Type::Float && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = F_MULT;
				lhs = a;
				rhs = new ToFloat_ast(b);
			}else {op = F_MULT; lhs = a; rhs = b;}
		}else {op =c; lhs = a; rhs = b;}
	}else if(c == DIV){
		if(t->tag == Type::Error){op =c; lhs = a; rhs = b;}
		else if (a->t->basetype == Type::Int && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = DIV;
				lhs = a;
				rhs = b;
			}else {op = F_DIV; lhs = new ToFloat_ast(a); rhs = b;}
		}
		else if(a->t->basetype == Type::Float && (b->t->basetype == Type::Int || b->t->basetype == Type::Float)){
			if(b->t->basetype == Type::Int){
				op = F_DIV;
				lhs = a;
				rhs = new ToFloat_ast(b);
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
void OP_ast::generate_code(){
	rhs->generate_code();
	lhs->generate_code();
	std::string s1,s2;
	switch(op){
		case OR :
			s1 = newLabel();
			s2 = newLabel();
			if(lhs->t->basetype == Type::Int && rhs->t->basetype == Type::Int){
				genCode(indent+"loadi(ind(esp),eax);");
				genCode(indent+"loadi(ind(esp,4),ebx);");
				genCode(indent+"popi(1);");
				genCode(indent+"popi(1);");
				genCode(indent+"cmpi(0,eax);");
				genCode(indent+"jne("+s1+");");
				genCode(indent+"cmpi(0,ebx);");
				genCode(indent+"jne("+s1+");");
				genCode(indent+"pushi(0);");
				genCode(indent+"j("+s2+");");
				genCode(s1+":");
				genCode(indent+"pushi(1);");
				genCode(s2+":");
			}else if(lhs->t->basetype == Type::Int && rhs->t->basetype == Type::Float){
				genCode(indent+"loadi(ind(esp),eax);");
				genCode(indent+"loadf(ind(esp,4),ebx);");
				genCode(indent+"popi(1);");
				genCode(indent+"popf(1);");
				genCode(indent+"cmpi(0,eax);");
				genCode(indent+"jne("+s1+");");
				genCode(indent+"cmpf(0,ebx);");
				genCode(indent+"jne("+s1+");");
				genCode(indent+"pushi(0);");
				genCode(indent+"j("+s2+");");
				genCode(s1+":");
				genCode(indent+"pushi(1);");
				genCode(s2+":");
			}else if(lhs->t->basetype == Type::Float && rhs->t->basetype == Type::Int){
				genCode(indent+"loadf(ind(esp),eax);");
				genCode(indent+"loadi(ind(esp,4),ebx);");
				genCode(indent+"popf(1);");
				genCode(indent+"popi(1);");
				genCode(indent+"cmpf(0,eax);");
				genCode(indent+"jne("+s1+");");
				genCode(indent+"cmpi(0,ebx);");
				genCode(indent+"jne("+s1+");");
				genCode(indent+"pushi(0);");
				genCode(indent+"j("+s2+");");
				genCode(s1+":");
				genCode(indent+"pushi(1);");
				genCode(s2+":");
			}else{
				genCode(indent+"loadf(ind(esp),eax);");
				genCode(indent+"loadf(ind(esp,4),ebx);");
				genCode(indent+"popf(1);");
				genCode(indent+"popf(1);");
				genCode(indent+"cmpf(0,eax);");
				genCode(indent+"jne("+s1+");");
				genCode(indent+"cmpf(0,ebx);");
				genCode(indent+"jne("+s1+");");
				genCode(indent+"pushi(0);");
				genCode(indent+"j("+s2+");");
				genCode(s1+":");
				genCode(indent+"pushi(1);");
				genCode(s2+":");
			}
			break;
		case AND :
			s1 = newLabel();
			s2 = newLabel();
			if(lhs->t->basetype == Type::Int && rhs->t->basetype == Type::Int){
				genCode(indent+"loadi(ind(esp),eax);");
				genCode(indent+"loadi(ind(esp,4),ebx);");
				genCode(indent+"popi(1);");
				genCode(indent+"popi(1);");
				genCode(indent+"cmpi(0,eax);");
				genCode(indent+"je("+s1+");");
				genCode(indent+"cmpi(0,ebx);");
				genCode(indent+"je("+s1+");");
				genCode(indent+"pushi(1);");
				genCode(indent+"j("+s2+");");
				genCode(s1+":");
				genCode(indent+"pushi(0);");
				genCode(s2+":");
			}else if(lhs->t->basetype == Type::Int && rhs->t->basetype == Type::Float){
				genCode(indent+"loadi(ind(esp),eax);");
				genCode(indent+"loadf(ind(esp,4),ebx);");
				genCode(indent+"popi(1);");
				genCode(indent+"popf(1);");
				genCode(indent+"cmpi(0,eax);");
				genCode(indent+"je("+s1+");");
				genCode(indent+"cmpf(0,ebx);");
				genCode(indent+"je("+s1+");");
				genCode(indent+"pushi(1);");
				genCode(indent+"j("+s2+");");
				genCode(s1+":");
				genCode(indent+"pushi(0);");
				genCode(s2+":");
			}else if(lhs->t->basetype == Type::Float && rhs->t->basetype == Type::Int){
				genCode(indent+"loadf(ind(esp),eax);");
				genCode(indent+"loadi(ind(esp,4),ebx);");
				genCode(indent+"popf(1);");
				genCode(indent+"popi(1);");
				genCode(indent+"cmpf(0,eax);");
				genCode(indent+"je("+s1+");");
				genCode(indent+"cmpi(0,ebx);");
				genCode(indent+"je("+s1+");");
				genCode(indent+"pushi(1);");
				genCode(indent+"j("+s2+");");
				genCode(s1+":");
				genCode(indent+"pushi(0);");
				genCode(s2+":");
			}else{
				genCode(indent+"loadf(ind(esp),eax);");
				genCode(indent+"loadf(ind(esp,4),ebx);");
				genCode(indent+"popf(1);");
				genCode(indent+"popf(1);");
				genCode(indent+"cmpf(0,eax);");
				genCode(indent+"je("+s1+");");
				genCode(indent+"cmpf(0,ebx);");
				genCode(indent+"je("+s1+");");
				genCode(indent+"pushi(1);");
				genCode(indent+"j("+s2+");");
				genCode(s1+":");
				genCode(indent+"pushi(0);");
				genCode(s2+":");
			}
			break;
		case EQ_OP :
			s1 = newLabel();
			s2 = newLabel();
			if(lhs->t->basetype == Type::Int && rhs->t->basetype == Type::Int){
				genCode(indent+"loadi(ind(esp),eax);");
				genCode(indent+"loadi(ind(esp,4),ebx);");
				genCode(indent+"popi(1);");
				genCode(indent+"popi(1);");
				genCode(indent+"cmpi(ebx,eax);");
				genCode(indent+"jne("+s1+");");
				genCode(indent+"pushi(1);");
				genCode(indent+"j("+s2+");");
				genCode(s1+":");
				genCode(indent+"pushi(0);");
				genCode(s2+":");
			}else if(lhs->t->basetype == Type::Int && rhs->t->basetype == Type::Float){
				genCode(indent+"loadi(ind(esp),eax);");
				genCode(indent+"loadf(ind(esp,4),ebx);");
				genCode(indent+"popi(1);");
				genCode(indent+"popf(1);");
				genCode(indent+ "intTofloat(eax);");
				genCode(indent+"cmpf(ebx,eax);");
				genCode(indent+"jne("+s1+");");
				genCode(indent+"pushi(1);");
				genCode(indent+"j("+s2+");");
				genCode(s1+":");
				genCode(indent+"pushi(0);");
				genCode(s2+":");
			}else if(lhs->t->basetype == Type::Float && rhs->t->basetype == Type::Int){
				genCode(indent+"loadf(ind(esp),eax);");
				genCode(indent+"loadi(ind(esp,4),ebx);");
				genCode(indent+"popf(1);");
				genCode(indent+"popi(1);");
				genCode(indent+ "intTofloat(ebx);");
				genCode(indent+"cmpf(ebx,eax);");
				genCode(indent+"jne("+s1+");");
				genCode(indent+"pushi(1);");
				genCode(indent+"j("+s2+");");
				genCode(s1+":");
				genCode(indent+"pushi(0);");
				genCode(s2+":");
			}else{
				genCode(indent+"loadf(ind(esp),eax);");
				genCode(indent+"loadf(ind(esp,4),ebx);");
				genCode(indent+"popf(1);");
				genCode(indent+"popf(1);");
				genCode(indent+"cmpf(ebx,eax);");
				genCode(indent+"jne("+s1+");");
				genCode(indent+"pushi(1);");
				genCode(indent+"j("+s2+");");
				genCode(s1+":");
				genCode(indent+"pushi(0);");
				genCode(s2+":");
			}
			break;
		case NE_OP : 
			s1 = newLabel();
			s2 = newLabel();
			if(lhs->t->basetype == Type::Int && rhs->t->basetype == Type::Int){
				genCode(indent+"loadi(ind(esp),eax);");
				genCode(indent+"loadi(ind(esp,4),ebx);");
				genCode(indent+"popi(1);");
				genCode(indent+"popi(1);");
				genCode(indent+"cmpi(ebx,eax);");
				genCode(indent+"je("+s1+");");
				genCode(indent+"pushi(1);");
				genCode(indent+"j("+s2+");");
				genCode(s1+":");
				genCode(indent+"pushi(0);");
				genCode(s2+":");
			}else if(lhs->t->basetype == Type::Int && rhs->t->basetype == Type::Float){
				genCode(indent+"loadi(ind(esp),eax);");
				genCode(indent+"loadf(ind(esp,4),ebx);");
				genCode(indent+"popi(1);");
				genCode(indent+"popf(1);");
				genCode(indent+ "intTofloat(eax);");
				genCode(indent+"cmpf(ebx,eax);");
				genCode(indent+"je("+s1+");");
				genCode(indent+"pushi(1);");
				genCode(indent+"j("+s2+");");
				genCode(s1+":");
				genCode(indent+"pushi(0);");
				genCode(s2+":");
			}else if(lhs->t->basetype == Type::Float && rhs->t->basetype == Type::Int){
				genCode(indent+"loadf(ind(esp),eax);");
				genCode(indent+"loadi(ind(esp,4),ebx);");
				genCode(indent+"popf(1);");
				genCode(indent+"popi(1);");
				genCode(indent+ "intTofloat(ebx);");
				genCode(indent+"cmpf(ebx,eax);");
				genCode(indent+"je("+s1+");");
				genCode(indent+"pushi(1);");
				genCode(indent+"j("+s2+");");
				genCode(s1+":");
				genCode(indent+"pushi(0);");
				genCode(s2+":");
			}else{
				genCode(indent+"loadf(ind(esp),eax);");
				genCode(indent+"loadf(ind(esp,4),ebx);");
				genCode(indent+"popf(1);");
				genCode(indent+"popf(1);");
				genCode(indent+"cmpf(ebx,eax);");
				genCode(indent+"je("+s1+");");
				genCode(indent+"pushi(1);");
				genCode(indent+"j("+s2+");");
				genCode(s1+":");
				genCode(indent+"pushi(0);");
				genCode(s2+":");
			}
			break;
		case GT : 
			s1 = newLabel();
			s2 = newLabel();
			genCode(indent+"loadi(ind(esp),eax);");
			genCode(indent+"loadi(ind(esp,4),ebx);");
			genCode(indent+"popi(1);");
			genCode(indent+"popi(1);");
			genCode(indent+"cmpi(ebx,eax);");
			genCode(indent+"jge("+s1+");");
			genCode(indent+"pushi(1);");
			genCode(indent+"j("+s2+");");
			genCode(s1+":");
			genCode(indent+"pushi(0);");
			genCode(s2+":");
			break;
		case F_GT :
			s1 = newLabel();
			s2 = newLabel();
			genCode(indent+"loadf(ind(esp),eax);");
			genCode(indent+"loadf(ind(esp,4),ebx);");
			genCode(indent+"popf(1);");
			genCode(indent+"popf(1);");
			genCode(indent+"cmpf(ebx,eax);");
			genCode(indent+"jge("+s1+");");
			genCode(indent+"pushi(1);");
			genCode(indent+"j("+s2+");");
			genCode(s1+":");
			genCode(indent+"pushi(0);");
			genCode(s2+":");
			break;
		case LT : 
			s1 = newLabel();
			s2 = newLabel();
			genCode(indent+"loadi(ind(esp),eax);"); //lhs
			genCode(indent+"loadi(ind(esp,4),ebx);"); // rhs
			genCode(indent+"popi(1);");
			genCode(indent+"popi(1);");
			genCode(indent+"cmpi(ebx,eax);");
			genCode(indent+"jle("+s1+");");
			genCode(indent+"pushi(1);");
			genCode(indent+"j("+s2+");");
			genCode(s1+":");
			genCode(indent+"pushi(0);");
			genCode(s2+":");
			break;
		case F_LT :
			s1 = newLabel();
			s2 = newLabel();
			genCode(indent+"loadf(ind(esp),eax);");
			genCode(indent+"loadf(ind(esp,4),ebx);");
			genCode(indent+"popf(1);");
			genCode(indent+"popf(1);");
			genCode(indent+"cmpf(ebx,eax);");
			genCode(indent+"jle("+s1+");");
			genCode(indent+"pushi(1);");
			genCode(indent+"j("+s2+");");
			genCode(s1+":");
			genCode(indent+"pushi(0);");
			genCode(s2+":");
			break;
		case LE_OP : 
			s1 = newLabel();
			s2 = newLabel();
			genCode(indent+"loadi(ind(esp),eax);");
			genCode(indent+"loadi(ind(esp,4),ebx);");
			genCode(indent+"popi(1);");
			genCode(indent+"popi(1);");
			genCode(indent+"cmpi(ebx,eax);");
			genCode(indent+"jl("+s1+");");
			genCode(indent+"pushi(1);");
			genCode(indent+"j("+s2+");");
			genCode(s1+":");
			genCode(indent+"pushi(0);");
			genCode(s2+":");
			break;
		case F_LE_OP :
			s1 = newLabel();
			s2 = newLabel();
			genCode(indent+"loadf(ind(esp),eax);");
			genCode(indent+"loadf(ind(esp,4),ebx);");
			genCode(indent+"popf(1);");
			genCode(indent+"popf(1);");
			genCode(indent+"cmpf(ebx,eax);");
			genCode(indent+"jl("+s1+");");
			genCode(indent+"pushi(1);");
			genCode(indent+"j("+s2+");");
			genCode(s1+":");
			genCode(indent+"pushi(0);");
			genCode(s2+":");
			break;
		case GE_OP : 
			s1 = newLabel();
			s2 = newLabel();
			genCode(indent+"loadi(ind(esp),eax);");
			genCode(indent+"loadi(ind(esp,4),ebx);");
			genCode(indent+"popi(1);");
			genCode(indent+"popi(1);");
			genCode(indent+"cmpi(ebx,eax);");
			genCode(indent+"jg("+s1+");");
			genCode(indent+"pushi(1);");
			genCode(indent+"j("+s2+");");
			genCode(s1+":");
			genCode(indent+"pushi(0);");
			genCode(s2+":");
			break;
		case F_GE_OP :
			s1 = newLabel();
			s2 = newLabel();
			genCode(indent+"loadf(ind(esp),eax);");
			genCode(indent+"loadf(ind(esp,4),ebx);");
			genCode(indent+"popf(1);");
			genCode(indent+"popf(1);");
			genCode(indent+"cmpf(ebx,eax);");
			genCode(indent+"jg("+s1+");");
			genCode(indent+"pushi(1);");
			genCode(indent+"j("+s2+");");
			genCode(s1+":");
			genCode(indent+"pushi(0);");
			genCode(s2+":");
			break;
		case PLUS : 
			genCode(indent+"loadi(ind(esp),eax);");
			genCode(indent+"loadi(ind(esp,4),ebx);");
			genCode(indent+"popi(1);");
			genCode(indent+"popi(1);");
			genCode(indent+"addi(ebx,eax);");
			genCode(indent+"pushi(eax);");
			break;
		case F_PLUS :
			genCode(indent+"loadf(ind(esp),eax);");
			genCode(indent+"loadf(ind(esp,4),ebx);");
			genCode(indent+"popf(1);");
			genCode(indent+"popf(1);");
			genCode(indent+"addf(ebx,eax);");
			genCode(indent+"pushf(eax);");
			break;
		case MINUS : 
			genCode(indent+"loadi(ind(esp),eax);");
			genCode(indent+"loadi(ind(esp,4),ebx);");
			genCode(indent+"popi(1);");
			genCode(indent+"popi(1);");
			genCode(indent+"muli(-1,ebx);");
			genCode(indent+"addi(ebx,eax);");
			genCode(indent+"pushi(eax);");
			break;
		case F_MINUS :
			genCode(indent+"loadf(ind(esp),eax);");
			genCode(indent+"loadf(ind(esp,4),ebx);");
			genCode(indent+"popf(1);");
			genCode(indent+"popf(1);");
			genCode(indent+"mulf(-1,ebx);");
			genCode(indent+"addf(ebx,eax);");
			genCode(indent+"pushf(eax);");
			break;
		case MULT : 
			genCode(indent+"loadi(ind(esp),eax);");
			genCode(indent+"loadi(ind(esp,4),ebx);");
			genCode(indent+"popi(1);");
			genCode(indent+"popi(1);");
			genCode(indent+"muli(ebx,eax);");
			genCode(indent+"pushi(eax);");
			break;
		case F_MULT :
			genCode(indent+"loadf(ind(esp),eax);");
			genCode(indent+"loadf(ind(esp,4),ebx);");
			genCode(indent+"popf(1);");
			genCode(indent+"popf(1);");
			genCode(indent+"mulf(ebx,eax);");
			genCode(indent+"pushf(eax);");
			break;
		case DIV : 
			genCode(indent+"loadi(ind(esp),ebx);");
			genCode(indent+"loadi(ind(esp,4),eax);");
			genCode(indent+"popi(1);");
			genCode(indent+"popi(1);");
			genCode(indent+"divi(ebx,eax);");
			genCode(indent+"pushi(eax);");
			break;
		case F_DIV :
			genCode(indent+"loadf(ind(esp),ebx);");
			genCode(indent+"loadf(ind(esp,4),eax);");
			genCode(indent+"popf(1);");
			genCode(indent+"popf(1);");
			genCode(indent+"divf(ebx,eax);");
			genCode(indent+"pushf(eax);");
			break;
		default :
			if(lhs->t->basetype == Type::Int){
				genCode(indent+"loadi(ind(esp,4),eax);");
				genCode(indent+"storei(eax,ind(ebp,edx));");
				genCode(indent+"popi(1);");
				genCode(indent+"popi(1);");
				genCode(indent+"pushi(eax);");
			}else if(lhs->t->basetype == Type::Float){
				genCode(indent+"loadf(ind(esp,4),eax);");
				genCode(indent+"storef(eax,ind(ebp,edx));");
				genCode(indent+"popf(1);");
				genCode(indent+"popf(1);");
				genCode(indent+"pushf(eax);");
			}
	}
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
void otherOP_ast::generate_code(){
	std::string s1,s2;
	switch(op){
	case UMINUS : 
		if(t->basetype == Type::Int){
			exp_0->generate_code();
			genCode(indent + "loadi(ind(esp),eax);");
			genCode(indent+ "muli(-1,eax);");
			genCode(indent+"storei(eax,ind(esp));");
		}else if(t->basetype == Type::Float){
			exp_0->generate_code();
			genCode(indent + "loadf(ind(esp),eax);");
			genCode(indent+ "mulf(-1,eax);");
			genCode(indent+"storef(eax,ind(esp));");
		}
		break;
	case NOT : 
		s1 = newLabel();
		s2 = newLabel();
		if(t->basetype == Type::Int){
			exp_0->generate_code();
			genCode(indent + "loadi(ind(esp),eax);");
			genCode(indent+ "cmpi(0,eax);");
			genCode(indent+"jne("+s1+");");
			genCode(indent+"storei(1,ind(esp));");
			genCode(indent+"jne("+s2+");");
			genCode(s1+":");
			genCode(indent+"storei(0,ind(esp));");
			genCode(s2+":");
		}else if(t->basetype == Type::Float){
			exp_0->generate_code();
			genCode(indent + "loadf(ind(esp),eax);");
			genCode(indent+"popf(1);");
			genCode(indent+ "cmpf(0,eax);");
			genCode(indent+"jne("+s1+");");
			genCode(indent+"storei(1,ind(esp));");
			genCode(indent+"jne("+s2+");");
			genCode(s1+":");
			genCode(indent+"pushi(0)");
			genCode(s2+":");
		}
		break;
	case PP : 
		if(t->basetype == Type::Int){
			exp_0->generate_code();
			genCode(indent+"loadi(ind(esp),eax);");
			genCode(indent+"popi(1);");
			genCode(indent+"move(eax,ebx);");
			genCode(indent+"addi(1,eax);");
			genCode(indent+"storei(eax,ind(ebp,edx));");
			genCode(indent+"pushi(ebx);");
		}else if(t->basetype == Type::Float){
			exp_0->generate_code();
			genCode(indent+"loadf(ind(esp),eax);");
			genCode(indent+"popf(1);");
			genCode(indent+"move(eax,ebx);");
			genCode(indent+"addf(1,eax);");
			genCode(indent+"storef(eax,ind(ebp,edx));");
			genCode(indent+"pushf(ebx);");
		}
		break;
	}
}
funcall_ast::funcall_ast(){
	args = NULL;
}
funcall_ast::funcall_ast(std::string s){
	func_name = s;
	identifier_ast* idast = new identifier_ast(s);
	args = new std::list<Exp_ast*>;
	args->push_back(idast);
}
funcall_ast::funcall_ast(std::string s,std::list<Exp_ast*>* a){
	func_name = s;
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
void funcall_ast::generate_code(){
	Type* temp = (globalST.lookup(func_name))->returntype;
	std::list<Exp_ast*>::iterator it;
	it = args->begin();
	++it;
	if(temp->basetype == Type::Int){
		genCode(indent+"pushi(0); // for _storing return value");
	}else if(temp->basetype == Type::Float){
		genCode(indent+"pushf(0);  // for _storing return value");
	}
	for(;it!=args->end();++it){
		(*it)->generate_code();
	}
	genCode(indent+func_name+"();");
	--it;
	for(;it!=args->begin();--it){
		if((*it)->t->basetype == Type::Int){
			genCode(indent+"popi(1);");
		}else if((*it)->t->basetype == Type::Float){
			genCode(indent+"popf(1);");
		} 
	}
}
float_ast::float_ast(float a){
	value = a;
}
void float_ast::print(){
	std::cout<<"(FLOATconst "<<value<<" )";
}
void float_ast::generate_code(){
	genCode(indent+"pushf(" + std::to_string(value) +");");
}
int_ast::int_ast(int a){
	value = a;
}
void int_ast::print(){
	std::cout<<"(INTconst "<<value<<" )";
}
void int_ast::generate_code(){
	genCode(indent+"pushi(" + std::to_string(value) +");");
}
strconstant_ast::strconstant_ast(std::string st){
	s= st;
}
void strconstant_ast::print(){
	std::cout<<"(StrConst "<<s<<" )";
}
void strconstant_ast::generate_code(){
	//TODO
}
identifier_ast::identifier_ast(){
	id ="";
}
identifier_ast::identifier_ast(std::string s){
	id = s;
}
void identifier_ast::print(){
	std::cout<<"(Identifier "<<id<<" )";
}
void identifier_ast::generate_code(){
	//TODO
}
array_identifier_ast::array_identifier_ast(){
	id ="";
}
array_identifier_ast::array_identifier_ast(std::string s){
	id =s;
	t = new Type();
}
void array_identifier_ast::print(){
	std::cout<<"(Identifier "<<id<<" )";
}
void array_identifier_ast::generate_code(){
	if(t->tag == Type::Base && t->basetype == Type::Int){
 		genCode(indent+"loadi(ind(ebp,"+ std::to_string(offset)+"),eax);");
 		genCode(indent+"pushi(eax);");
 		genCode(indent+"move("+std::to_string(offset)+",edx);");
 	}else if (t->tag == Type::Base && t->basetype == Type::Float){
 		genCode(indent+"loadf(ind(ebp,"+ std::to_string(offset)+"),eax);");
 		genCode(indent+"pushf(eax);");
 		genCode(indent+"move("+std::to_string(offset)+",edx);");
 	}else{
 		genCode(indent+"addi("+std::to_string(offset)+",edx);");
 	}
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
void index_ast::generate_code(){
	ex->generate_code();
	if(t->basetype == Type::Int){
		genCode(indent+"loadi(ind(esp,0),eax);");
		genCode(indent+"popi(1);");
		genCode(indent+"muli("+std::to_string(-4)+",eax);");
		genCode(indent +"move(eax,edx);");
		ref->t->width = 4 * ref->t->size;
		ref->generate_code();
		genCode(indent+"loadi(ind(ebp,edx),eax);");
 		genCode(indent+"pushi(eax);");
 	}else if (t->basetype == Type::Float){
 		genCode(indent+"loadi(ind(esp,0),eax);");
		genCode(indent+"popi(1);");
		genCode(indent+"muli("+std::to_string(-t->width)+",eax);");
		genCode(indent +"move(eax,edx);");
		ref->t->width = 4 * ref->t->size;
		ref->generate_code();
		genCode(indent+"loadf(ind(ebp,edx),eax);");
 		genCode(indent+"pushf(eax);");
 	}else{
 		genCode(indent+"loadi(ind(esp,0),eax);");
		genCode(indent+"popi(1);");
		genCode(indent+"muli("+std::to_string(-t->width)+",eax);");
		genCode(indent +"addi(eax,edx)");
		ref->t->width = t->width * ref->t->size;
		ref->generate_code();
 	}

}
ToFloat_ast::ToFloat_ast(){
	exp_0 = NULL;
}
ToFloat_ast::ToFloat_ast(Exp_ast* a){
	exp_0 = a;
}
void ToFloat_ast::print(){
	std::cout<<" ( To_Float ";
	exp_0->print();
	std::cout<<" ) ";
}
void ToFloat_ast::generate_code(){
	exp_0->generate_code();
	genCode(indent+"loadi(ind(esp,0),eax);");
	genCode(indent+"popi(1);");
	genCode(indent+"intTofloat(eax);");
	genCode(indent+"pushf(eax);");
}
ToInt_ast::ToInt_ast(){
	exp_0 = NULL;
}
ToInt_ast::ToInt_ast(Exp_ast* a){
	exp_0 = a;
}
void ToInt_ast::print(){
	std::cout<<" ( To_Int ";
	exp_0->print();
	std::cout<<" ) ";
}
void ToInt_ast::generate_code(){
	exp_0->generate_code();
	genCode(indent+"loadf(ind(esp,0),eax);");
	genCode(indent+"popf(1);");
	genCode(indent+"floatToint(eax);");
	genCode(indent+"pushi(eax);");
}
