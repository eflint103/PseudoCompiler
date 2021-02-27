#include <iostream>
#include <string>
#include <vector>
#include "val.h"
using namespace std;

Value Value::operator+(const Value& op) const{
    if(IsInt() && op.IsInt()){
        return Value(I+op.I);
    }
    else if(IsStr() && op.IsStr()){
        return Value(GetStr()+op.GetStr());
    }
    else{
        return Value();
    }
}
    
Value Value::operator-(const Value& op) const{
    if(IsInt() && op.IsInt()){
        return Value(I-op.I);
    }
    else if(IsStr() && op.IsStr()){
        string tempword="";
        bool check = true;
        string firstword=GetStr();
        
        string secword=op.GetStr();
        char secwordc=secword[0];
        int l1=firstword.length();
        int l2=secword.length();
        if (l2!=1){
            for(int i=0;i<(l1-l2);i++){
                tempword+=firstword[i];
            }
                
            return Value(tempword);
        }
        else{
            
        for (int i=0;i<l1;i++){
            if(firstword[i]==secwordc &&check){
                check=false;
            }
            else{
                tempword+=firstword[i];
            }
        }
        
        return Value(tempword);
        }
    }
    else{
        return Value();
    }
}

Value Value::operator*(const Value& op) const{
    if(IsInt() && op.IsInt()){
        return Value(I*op.I);
    }
    else if(IsStr() && op.IsInt()){
        string tempMult="";
        for (int i=0;i<op.GetInt();i++){
            tempMult+=GetStr();
        }
        return Value(tempMult);
    }
    else if(IsInt()&&op.IsStr()){
        string tempMult="";
        for (int i=0;i<GetInt();i++){
            tempMult+=op.GetStr();
        }
        return Value(tempMult);
    }
    else if(IsStr() && op.IsStr()){
        return Value();
    }
    else{
        return Value();
    }
}

Value Value::operator/(const Value& op) const{
    if(IsInt() && op.IsInt()){
        return Value(I/op.I);
    }
    else if(IsStr() && op.IsStr()){
        return Value(GetStr());
    }
    else{
        return Value();
    }
}
