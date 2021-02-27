#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <string>
#include <map>
#include "lex.h"
#include "parse.h"
#include "val.h"

using namespace std;
void
doOp(const Value& a, const Value& b) {
	//cout << a << " + " << b << " is " << flush << a + b << endl;
	//cout << a << " - " << b << " is " << flush << a - b << endl;
    //cout << a << " * " << b << " is " << flush << a * b << endl;
    //cout << a << " / " << b << " is " << flush << a / b << endl;
}
int e;

int main(int argc, char *argv[])
{
   
    istream* in = &cin;
    ifstream file;
  
    
    
    for( int i=1; i<argc; i++ ) {
        string arg( argv[i] );
        if( arg== "-v" ){}
            //vflag= true;
        else if( arg== "-ids" ){}
            //idsflag= true;
        else if( arg== "-iconsts" ){}
            //icflag= true;
        else if( arg== "-sconsts" ){}
            //scflag= true;
        else if( arg[0] == '-' ) {
            cerr<< "UNRECOGNIZED FLAG " << arg<< endl;
            return 0;
        }
        else if( in != &cin) {
            cerr<< "ONLY ONE FILE NAME ALLOWED" << endl;
            return 0;
        }
        else {
            file.open(arg);
            if( file.is_open() == false ) {
                cerr<< "CANNOT OPEN " << arg<< endl;
                return 0;
            }
            in = &file;
        }
    }

    map<string,Value> symbols;
    
    int linenum=1;
    Pt *prog = Prog(*in, linenum);
    if (prog==0){
        return -1;
    }
    
    try {
       prog->Eval(symbols);
    }
    
    catch( string er ) {}
     /*   if (er=="mult"){
            cout<<"RUNTIME ERROR at "<<e<<": Type error on multiplication"<< endl;
            return -1;
        }
         if (er=="varunkown"){
            cout<<"RUNTIME ERROR at "<< e <<": Variable used but not assigned"<< endl;
            return -1;
         }
         if (er=="intex"){
            cout<<"RUNTIME ERROR at "<< e <<": control expression not an integer"<< endl;
            return -1; 
         }

    }*/
    
    
    /*extern int pc;
    extern int ec;
    extern int md2;
    cout<<"PLUS COUNT: "<<pc<<endl;
    cout<<"EQ COUNT: "<<ec<<endl;
    cout<<"MAX DEPTH: "<< md2<<endl;*/
///////////////////////////
    ///////////////////////
    
    
    return 0;
}
    
    
    