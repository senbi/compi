#include <iostream>
#include <string>
#include <list>
class abstract_astnode
{
	public:
	virtual void print () = 0;
	Type* t;
	virtual void generate_code() = 0;
/*	virtual std::string generate_code(const symbolTable&) = 0;
	virtual basic_types getType() = 0;
	virtual bool checkTypeofAST() = 0;
	protected:
	virtual void setType(basic_types) = 0;
	private:
	typeExp astnode_type;
*/
};
class stmt_astnode : public abstract_astnode 
	/* defination of abstract statment class */ 
{
public:
	virtual void print() = 0 ;
	std::vector<int> nextlist;
//private:
};
class Exp_ast : public abstract_astnode
{
	public:
		virtual void print()= 0;
};


class seq_astnode : public stmt_astnode
{
public:
	stmt_astnode* stmt_0;
	stmt_astnode* stmt_1;
	seq_astnode();
	seq_astnode(stmt_astnode*,stmt_astnode*);
	void print() ;
	void generate_code();

};
class blockStatement_astnode : public stmt_astnode
{
	public:
		std::list<stmt_astnode*>*l;
		blockStatement_astnode();
		blockStatement_astnode(std::list<stmt_astnode*>*);
		void insert(stmt_astnode*);
		void print();
		void generate_code();
};
class ass_astnode : public stmt_astnode // it can be null
{
public:
	Exp_ast* exp_0;
	Exp_ast* exp_1; 
	ass_astnode();
	ass_astnode(Exp_ast*,Exp_ast*);
	void print() ;
	void generate_code();
};

class return_astnode : public stmt_astnode
{
public:
	Exp_ast* exp_0;
	return_astnode();
	return_astnode(Exp_ast*);
	void print() ;
	void generate_code();
};
class funstmt_ast : public stmt_astnode
{
	public:
	std::list<Exp_ast*> *args;
	std::string func_name;
	funstmt_ast();
	funstmt_ast(std::string s);
	funstmt_ast(std::string s,std::list<Exp_ast*>*);
	void insert(Exp_ast*);
	void print();
	void generate_code();
};

class if_astnode : public stmt_astnode
{
public:
	Exp_ast* exp_0;
	stmt_astnode* stmt_1;
	stmt_astnode* stmt_2;
	if_astnode();
	if_astnode(Exp_ast*,stmt_astnode*,stmt_astnode*);
	void print() ;
	void generate_code();
};

class while_astnode : public stmt_astnode
{
public:
	void print() ;
	void generate_code();
	Exp_ast* exp_0;
	stmt_astnode* stmt_1;
	while_astnode();
	while_astnode(Exp_ast*,stmt_astnode*);
};

class for_astnode : public stmt_astnode
{
public:
	Exp_ast* exp_0;
	Exp_ast* exp_1;
	Exp_ast* exp_2;
	stmt_astnode* stmt_3;
	for_astnode();
	for_astnode(Exp_ast*,Exp_ast*,Exp_ast*,stmt_astnode*);
	void print() ;
	void generate_code();
};
class OP_ast : public Exp_ast
{
	public:
		enum Kind{
			OR,AND,EQ_OP,NE_OP,GT,LT,
			LE_OP,GE_OP,PLUS,MINUS,MULT,DIV,ASSIGN,F_PLUS,F_MINUS,
			F_GE_OP,F_LE_OP,F_MULT,F_DIV,F_GT,F_LT,
		};
		Kind op;
		Exp_ast* lhs;
		Exp_ast* rhs;
	OP_ast();
	OP_ast(Exp_ast*,Exp_ast*,Kind);
	void print();
	void generate_code();

};
class otherOP_ast : public Exp_ast
{
	public:
		enum Kind{
			UMINUS,NOT,PP
		};
		Kind op;
		Exp_ast* exp_0;
	otherOP_ast();
	otherOP_ast(Exp_ast*,Kind);
	void print();
	void generate_code();
};
class funcall_ast : public Exp_ast
{
	public:
	std::list<Exp_ast*> *args;
	std::string func_name;
	funcall_ast();
	funcall_ast(std::string s);
	funcall_ast(std::string s,std::list<Exp_ast*>*);
	void insert(Exp_ast*);
	void print();
	void generate_code();
};

class float_ast : public Exp_ast
{
	public:
		float value;
		float_ast();
		float_ast(float);
		void print();
		void generate_code();
};
class int_ast : public Exp_ast
{
	public:
		int value;
		int_ast();
		int_ast(int);
		void print();
		void generate_code();
};
class strconstant_ast : public Exp_ast
{
	public:
		std::string s;
		strconstant_ast();
		strconstant_ast(std::string);
		void print();
		void generate_code();
};
class identifier_ast : public Exp_ast
{
	public:
		std::string id;
		identifier_ast();
		identifier_ast(std::string);
		void print();
		void generate_code();
};
class Arrayref_ast : public Exp_ast{
	public:
	int offset;
	virtual void print() = 0 ;
	virtual void generate_code() = 0;
};
class array_identifier_ast : public Arrayref_ast
{
	public:
		std::string id;
		array_identifier_ast();
		array_identifier_ast(std::string);
		void print();
		void generate_code();
};
class index_ast : public Arrayref_ast
{
	public:
		Arrayref_ast* ref;
		Exp_ast* ex;
	index_ast();
	index_ast(Arrayref_ast*,Exp_ast*);
	void print();
	void generate_code();

};
class ToFloat_ast : public Exp_ast{
	public:
		Exp_ast* exp_0;
	ToFloat_ast();
	ToFloat_ast(Exp_ast*);
	void print();
	void generate_code();
};
class ToInt_ast : public Exp_ast{
public:
	Exp_ast* exp_0;
	ToInt_ast();
	ToInt_ast(Exp_ast*);
	void print();
	void generate_code();
};
