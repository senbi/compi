#include <iostream>
#include <string>
#include <list>
#include <map>

class Type {
public:
	enum Kind {
		Base, Pointer,Array, Error, Ok
	};
	enum Basetype {
		Int, Float, Void
	};
	Kind tag;
	union {
		Basetype basetype;
		Type* pointed;
	};
	int size;
	Type();    // Default
	Type(Kind); // Error, Ok
	Type(Kind, Basetype); //Int, Float
	Type(Kind, Type*);   // Pointer
        void printType();
  //    ~Type();
};
class VarInfo{
public:
	Type* t;
	int offset;
	int width;
	VarInfo();
	VarInfo(Type*,int,int);
	void print();
};
class STable{
public:
	std::vector< std::pair<std::string , VarInfo*> > args;
	std::map<std::string , VarInfo*> lst;
	Type *returntype;
	int offset;
	STable();
	STable(std::string rtype);
	void insert_args(std::string var,VarInfo* v);
	void insert(std::string var,VarInfo* v);
	VarInfo* lookup(std::string var);
	void print();
};

class GSTable{
public:
	std::map<std::string,STable*> gst;
	void insert(std::string,STable*);
	STable* lookup(std::string );
};
class func_name{
public:
	std::string fname;
	func_name();
	func_name(std::string);
};
class func_tn{
public:
	func_name *f;
	Type *t;
	bool args;
	int offset;
	func_tn();
	func_tn(func_name*,std::string,bool ar);
	int update(int&);
};
class dec_ast{
public:
	Type *t;
	int length;
	std::string vname;
	dec_ast();
	dec_ast(std::string s,Type *t1,int l);
	dec_ast(dec_ast *t1,int l);
};
class const_ast{
public:
	Type*t;
	union
	{
		int value;
		float fvalue;
	};
	const_ast();
	const_ast(Type*,int);
	const_ast(Type*,float);
	
};
class check_par{
public:
	std::vector< std::pair<std::string , VarInfo*> > args;
	int argno;
	check_par(std::vector< std::pair<std::string , VarInfo*> >&);
};
extern int line_no;
extern GSTable globalST;
//Type* result(Type* t1, Type* t2) 