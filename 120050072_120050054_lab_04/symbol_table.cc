#include <iostream>
#include <string>
#include <list>
#include <map>
// #include "symbol_table.h"
Type::Type() :
		tag(Ok) {
}
Type::Type(Kind kindval) :
		tag(kindval) {
}

Type::Type(Kind kindval, Basetype btype) :
		tag(kindval), basetype(btype) {
}

Type::Type(Kind kindval, Type* ptd) :
		tag(kindval), pointed(ptd) {
}

void Type::printType()
{ 
  switch (tag) 
   {
   case Base: switch (basetype) 
     {
       case Int: std::cout << "Int";
       break;
       case Float: std::cout << "Float";
     };
    break;
  case Error:std::cout << "	Error ";
    break;
  case Ok:std::cout << "Ok ";
    break;
  case Pointer:std::cout << "Pointer to ";
    pointed->printType();
  case Array:std::cout<<"Array of (";
  	pointed->printType();
  	std::cout<<","<<size<<")";
  }}

bool equal(Type* t1, Type* t2) {
	if (t1->tag == Type::Base && t2->tag == Type::Base)
		return (t1->basetype == t2->basetype);
	else if (t1->tag == Type::Pointer && t2->tag == Type::Pointer)
		return equal(t1->pointed, t2->pointed);
}

bool compatible(Type* t1, Type* t2) {
	if ((t1->basetype == Type::Int || t1->basetype == Type::Float)
			&& (t2->basetype == Type::Int || t2->basetype == Type::Float))
		return 1;
	else
		return equal(t1, t2);
}
Type* result(Type* t1, Type* t2) {
	if (t1->basetype == Type::Int && (t2->basetype == Type::Int || t2->basetype == Type::Float)){
		if(t2->basetype == Type::Int){
			return new Type(Type::Base,Type::Int);
		}else return new Type(Type::Base,Type::Int);
	}
	else if(t1->basetype == Type::Float && (t2->basetype == Type::Int || t2->basetype == Type::Float)){
		return new Type(Type::Base,Type::Float);
	}
	else if(t1->tag == Type::Array || t2->tag == Type::Array)
	{
		return new Type(Type::Error);
	}
	else if(equal(t1, t2))
		return t1;
	else return new Type(Type::Error);
}

VarInfo::VarInfo(){
	t = NULL;
	offset = -1;
	width=-1;
}
VarInfo::VarInfo(Type* type,int oset,int w){
	t = type;
	offset = oset;
	width = w;
}
void VarInfo::print(){
		t->printType();
		printf(" %d %d\n",offset,width);
	}
STable::STable(){
	offset = 0;
}
STable::STable(std::string rtype){
	offset == 0;
	if(rtype == "VOID"){
		returntype =  new Type(Type::Base, Type::Void);
		//offset =0;
	}else if(rtype == "INT"){
		returntype =  new Type(Type::Base, Type::Int);
		//offset = 0;
	}else if(rtype == "FLOAT"){
		returntype =  new Type(Type::Base, Type::Float);
		//offset = 0;
	}
}
void STable::insert_args(std::string var,VarInfo* v){
	lst.insert(std::pair<std::string,VarInfo*>(var,v));
	args.push_back(std::pair<std::string,VarInfo*>(var,v));
}
void STable::insert(std::string var,VarInfo* v){
	lst.insert(std::pair<std::string,VarInfo*>(var,v));
}
VarInfo* STable::lookup(std::string var){
	std::map<std::string , VarInfo*>::iterator it;
	std::vector< std::pair<std::string , VarInfo*> >::iterator it_args;

	it = lst.find(var);
	if(it==lst.end()){
		for(it_args=args.begin();it_args!=args.end();it_args++)
		{
			if (var==it_args->first)
			{
				return it_args->second;
			}
		}
		return NULL;
	}else{
		return (it->second);
	}
}
void STable::print(){
		std::map<std::string , VarInfo*>::iterator it;
		int i=0;
		for(it = lst.begin();it != lst.end();it++){
			std::cout<<it->first<<" ";
			it->second->print();

		}
}
void GSTable::insert(std::string var,STable* lst){
	gst.insert(std::pair<std::string,STable*>(var,lst));
}
STable* GSTable::lookup(std::string var){
	std::map<std::string , STable*>::iterator it;
	it = gst.find(var);
	if(it==gst.end()){
		return NULL;
	}else{
		return it->second;
	}
}
func_name::func_name(std::string name){
	fname = name;
}
func_tn::func_tn(){
	t == NULL;
	f == NULL;
}
func_tn::func_tn(func_name *fn,std::string s,bool ar){
	args = ar;
	f = fn;
	if(s == "VOID"){
		t =  new Type(Type::Base, Type::Void);
		offset =0;
	}else if(s == "INT"){
		t =  new Type(Type::Base, Type::Int);
		offset = 4;
	}else if(s == "FLOAT"){
		t =  new Type(Type::Base, Type::Float);
		offset = 4;
	}
}
int func_tn::update(int& a){
	int x = a;
	if(equal(t,new Type(Type::Base, Type::Void))){

	}else{
		a+=offset;
	}
	return x;
}
dec_ast::dec_ast(){
	t = NULL;
	length = 0;
}
dec_ast::dec_ast(std::string s,Type* t1,int l){
	vname = s;
	t = t1;
	length = l;
}
dec_ast::dec_ast(dec_ast* t1,int l){
	vname = t1->vname;
	t = new Type(Type::Array,t1->t);
	t->size = l;
	length = l*t1->length;
}
const_ast::const_ast(){
	t=NULL;
}
const_ast::const_ast(Type*t1,int l){
	t=t1;
	value=l;
}
const_ast::const_ast(Type*t1,float l){
	t=t1;
	fvalue=l;
}
check_par::check_par(std::vector< std::pair<std::string , VarInfo*> >& a){
	args = a ;
	argno=0;
}
GSTable globalST;
std::string current_func;
int line_no = 1;