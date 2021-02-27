/*
 * pt.h
 */

#ifndef PT_H_
#define PT_H_

#include <iostream>
#include <vector>
#include <map>
#include <string>

using std::vector;
using std::map;
using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

#include "parse.h"
#include "lex.h"
#include "val.h"

extern int e;

class Pt {
protected:
	

public:
    int	linenum;
	Pt	*left;
	Pt	*right;
	Pt(int linenum, Pt *l = 0, Pt *r = 0)
		: linenum(linenum), left(l), right(r) {}
	
	virtual ~Pt() {
		delete left;
		delete right;
	}
	
	int GetLineNumber() const { return linenum; }

    virtual bool isPlus() const { return false; }
    virtual bool isEq() const { return false; }
	virtual bool isIdent() const { return false; }
	virtual bool isVar() const { return false; }
	virtual string getId() const { return ""; }
	int depth(Pt *temp);
    virtual Value Eval(map<string,Value>& syms) const = 0;
};

class StmtList : public Pt {
public:
	StmtList(Pt *l, Pt *r) : Pt(0, l, r) {}
    
    Value Eval(map<string,Value>& symbols) const{
        left->Eval(symbols);
        if( right ){
            right->Eval(symbols);
        }
        return Value(0);
    }
    
};

class Print : public Pt {
public:
	Print(int line, Pt *e) : Pt(line, e) {}
    
    Value Eval(map<string,Value>& symbols) const {
        Value val = left->Eval(symbols);
        
        cout <<val;
        return val;
    }
};

class Println : public Pt {
public:
	Println(int line, Pt *e) : Pt(line, e) {}
    
    Value Eval(map<string,Value>& symbols) const{//WorkOn
        Value val = left->Eval(symbols);
        cout<<val<<endl;
        return val;
    }

};

class Repeat : public Pt {
public:
   Repeat(int line, Pt *e) : Pt(line, e) {}

   Value Eval(map<string,Value>& symbols) const{//WorkOn
       int repetitions=0;
       Value rep = right->Eval(symbols);
       cout<<repetitions;
       if (rep.IsInt()){
            repetitions = rep.GetInt();
           
       }
       else{
            e=GetLineNumber();
            cout<<"RUNTIME ERROR at "<<e<< ": control expression not an integer"<<endl;
       }
       for (int i=0 ; i<repetitions;i++){
           left->Eval(symbols);
       }
       return Value();
    }  

};

class Assign : public Pt {
public:
	Assign(int line, Pt *l, Pt *r) : Pt(line, l, r) {}

    Value Eval(map<string,Value>& symbols) const{//WorkOn
        Value val= right->Eval(symbols);
        symbols[left->getId()]= val;
        return Value(0);
    }
};

class PlusExpr : public Pt {
public:
	PlusExpr(int line, Pt *l, Pt *r) : Pt(line, l, r) {}
	
    Value Eval(map<string,Value>& symbols) const{
        auto val1=left->Eval(symbols);
        auto val2=right->Eval(symbols);
        return val1+val2;
    }
};

class MinusExpr : public Pt {
public:
	MinusExpr(int line, Pt *l, Pt *r) : Pt(line, l, r) {}
	
    Value Eval(map<string,Value>& symbols) const{
        auto val1=left->Eval(symbols);
        auto val2=right->Eval(symbols);
        return val1-val2;
    }
};

class TimesExpr : public Pt {
public:
	TimesExpr(int line, Pt *l, Pt *r) : Pt(line, l, r) {}
	
    Value Eval(map<string,Value>& symbols) const{
        auto val1=left->Eval(symbols);
        auto val2=right->Eval(symbols);
        if (val1.IsStr()&&val2.IsStr()){
            e=GetLineNumber();
            cout<<"RUNTIME ERROR at "<<e<< ": Type error on multiplication"<<endl;
           
        }
        return val1*val2;
    }
};

class DivideExpr : public Pt {
public:
	DivideExpr(int line, Pt *l, Pt *r) : Pt(line, l, r) {}
	
    Value Eval(map<string,Value>& symbols) const{
        auto val1=left->Eval(symbols);
        auto val2=right->Eval(symbols);
        return val1/val2;
    }
    
};

class IConst : public Pt {
	int val;

public:
	IConst(Tok& t) : Pt(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
	}
    
    Value Eval(map<string,Value>& symbols) const{
        return Value(val);
    }
};

class SConst : public Pt {
	string val;

public:
	SConst(Tok& t) : Pt(t.GetLinenum()) {
		val = t.GetLexeme();
	}
    
    Value Eval(map<string,Value>& symbols) const{
        return Value(val);
    }
    
};

class Ident : public Pt {
	string val;

public:
	Ident(Tok& t) : Pt(t.GetLinenum()) {
		val = t.GetLexeme();
	}
    virtual string getId() const{
        return val;
    }
    Value Eval(map<string,Value>& symbols) const{//WorkOn
        map<string,Value>::iterator it;//lab9
        
            it=symbols.find(val);
            if (it==symbols.end()){
                e=GetLineNumber();
            	cout<<"RUNTIME ERROR at "<<e<< ": Variable used but not assigned"<<endl;
                
            }
        else{
            return it->second;
        }
        
        return Value();
    }
 
};

#endif /* PT_H_ */
