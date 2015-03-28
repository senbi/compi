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
	exp_0 = a;
	exp_1 = b;
}
void ass_astnode::print(){
	if(exp_0 == NULL)std::cout<<"(Empty)";
	else{
		std::cout<<"(Assign ";
		exp_0->print();
		exp_1->print();
		std::cout<<" )";
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
	lhs = a;
	rhs = b;
	op = c;
}
void OP_ast::print(){
	std::cout<<"(";
	switch(op){
		case OR : 
			std::cout<<"OR ";
			break;
		case AND : 
			std::cout<<"AND ";
			break;
		case EQ_OP : 
			std::cout<<"EQ ";
			break;
		case NE_OP : 
			std::cout<<"NE ";
			break;
		case GT : 
			std::cout<<"GT ";
			break;
		case LT : 
			std::cout<<"LT ";
			break;
		case LE_OP : 
			std::cout<<"LE ";
			break;
		case GE_OP : 
			std::cout<<"GE ";
			break;
		case PLUS : 
			std::cout<<"Plus ";
			break;
		case MINUS : 
			std::cout<<"Minus ";
			break;
		case MULT : 
			std::cout<<"Mult ";
			break;
		case DIV : 
			std::cout<<"Div ";
			break;
		default :
			std::cout<<"Assign_exp ";
	}
	lhs->print();
	rhs->print();
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