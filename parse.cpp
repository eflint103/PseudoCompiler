/*
 * parse.cpp
 */

#include "parse.h"

int ec=0;
int pc=0;
int md=0;
int md2=0;    
int emptyline=1;
int Pt::depth(Pt *temp)   //Used is CS116 
{
	if(temp==NULL)
	{
		return 0;
	}
	else
	{
	    return 1+std::max(depth(temp->left),depth(temp->right));
	}
}  
namespace Parser {
bool pushed_back = false;
Tok	pushed_token;
  

static Tok GetNextToken(istream& in, int& line) {
    if( pushed_back ) {
		pushed_back = false;
		return pushed_token;
	}
	return getNextToken(in, line);
}

static void PushBackToken(Tok& t) {
	if( pushed_back ) {
		abort();
	}
	pushed_back = true;
	pushed_token = t;
}

}

static int error_count = 0;

void
ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

Pt *Prog(istream& in, int& line)
{
	Pt *sl = Sl(in, line);
	if( sl == 0 )
		ParseError(line, "No statements in program");

	if( error_count )
		return 0;
	
    md2 = sl->depth(sl);
	
    return sl;
}

// Sl is a Statement followed by a Statement List
Pt *Sl(istream& in, int& line) {
	Pt *s = Stmt(in, line);
	if( s == 0 )
		return 0;
	
	if( Parser::GetNextToken(in, line) != SC ) {
		ParseError(line-1-emptyline, "Missing semicolon");
		delete s;
		return 0;
	}

	return new StmtList(s, Sl(in,line));
}

Pt *Stmt(istream& in, int& line) {

	Tok t = Parser::GetNextToken(in, line);
    if (t==PRINT){
       Pt *s = PrintStmt(in, line); 
        return s;
    }
    else if (t==PRINTLN){
       Pt *s = PrintlnStmt(in, line); 
        return s;
    }
    else if (t==IDENT){
        Parser::PushBackToken(t);
        Pt *s=Expr(in,line);
         return s;
    }
    else if (t==REPEAT){
        Pt *s=RepeatStmt(in,line);
        return s;
    }
        return 0;

}

Pt *PrintStmt(istream& in, int& line) {
	int l = line;

	Pt *ex = Expr(in, line);
	if( ex == 0 ) {
		ParseError(line, "Missing expression after print");
		return 0;
	}

	return new Print(l, ex);
}

Pt *PrintlnStmt(istream& in, int& line) {
	int l = line;

	Pt *ex = Expr(in, line);
	if( ex == 0 ) {
		ParseError(line, "Missing expression after print");
		return 0;
	}

	return new Println(l, ex);

}

Pt *RepeatStmt(istream& in, int& line) {
	int l = line;
    Pt *ex = Expr(in, line);
    if( ex == 0 ) {
		ParseError(line, "Missing expression after print");
		return 0;
	}
    
    
		Tok t = Parser::GetNextToken(in, line);

		if( t != BEGIN ) {
			Parser::PushBackToken(t);
            ParseError(line,"NO BEGIN");
			return 0;
		}
        

		Pt *t2 = Stmt(in, line);
		if( t2 == 0 ) {
			ParseError(line, "Missing expression after operator");
			return 0;
		}
        
        t= Parser::GetNextToken(in, line);
        if(t!=END){
        	Parser::PushBackToken(t);
            ParseError(line,"NO END");
			return 0;
        }
    
    
    return new Repeat(l,t2);

}

Pt *Expr(istream& in, int& line) {
    Pt *t1 = Sum(in, line);
   
	if( t1 == 0 ) {
		return 0;
	}

	while ( true ) {
		Tok t = Parser::GetNextToken(in, line);

		if( t != EQ ) {
			Parser::PushBackToken(t);
			return t1;
		}

		Pt *t2 = Sum(in, line);
		if( t2 == 0 ) {
			ParseError(line, "Missing expression after operator");
			return 0;
		}

		
            ec++;
			t1 =new Assign(t.GetLinenum(), t1, t2);
    }
}

Pt *Sum(istream& in, int& line) {
	
    Pt *t1 = Prod(in, line);
	if( t1 == 0 ) {
		return 0;
	}

	while ( true ) {
		Tok t = Parser::GetNextToken(in, line);

		if( t != PLUS && t != MINUS ) {
			Parser::PushBackToken(t);
			return t1;
		}

		Pt *t2 = Prod(in, line);
		if( t2 == 0 ) {
			ParseError(line, "Missing expression after operator");
			return 0;
		}

		if( t == PLUS ){
            pc++;
            
			t1 = new PlusExpr(t.GetLinenum(), t1, t2);
        }
		else{
            
			t1 = new MinusExpr(t.GetLinenum(), t1, t2);
        }
	}
}

Pt *Prod(istream& in, int& line) {
    Pt *t1 = Primary(in, line);
	if( t1 == 0 ) {
		return 0;
	}

	while ( true ) {
		Tok t = Parser::GetNextToken(in, line);

		if( t != STAR && t != SLASH ) {
			Parser::PushBackToken(t);
			return t1;
		}

		Pt *t2 = Primary(in, line);
		if( t2 == 0 ) {
			ParseError(line, "Missing expression after operator");
			return 0;
		}

		if( t == STAR ){
            
			t1 = new TimesExpr(t.GetLinenum(), t1, t2);
        }
		else{
            
			t1 = new DivideExpr(t.GetLinenum(), t1, t2);
        }
	}
}

Pt *Primary(istream& in, int& line) {
	Tok t = Parser::GetNextToken(in, line);

	if( t == IDENT ) {
		return new Ident(t);
	}
	else if( t == ICONST ) {
		return new IConst(t);
	}
    else if( t == SCONST ) {
		return new SConst(t);
	}
    
    else if (t == LPAREN){
        
        Pt *t1 = Expr(in,line);
        
        if( t1 == 0 ){
		ParseError(line, "No statements in program");
        return 0;
        }
        Tok t2 = Parser::GetNextToken(in, line);
        if (t2==RPAREN){
            return t1;
        }
        else{ 
            Parser::PushBackToken(t2);
            ParseError(line, "RPAREN");
            return 0;
        }

    }
    else{
        return 0;
    }
    	 return 0; 

}

