#include <iostream>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <list>

using namespace std;

#define ln_operator '#'

class Expr{
public:
    bool is_primitive;
    char var;
    int cons;
    Expr* L,*R;
    list<Expr*> daughters;
    virtual void print()=0;
    virtual Expr*deriv(char wrt_var)=0;
    virtual Expr*simplify()=0;
    Expr(){};
    Expr(bool b){};
    Expr(char v):var(v){};
    Expr(int i):cons(i){};
    Expr(string input){};
    Expr(Expr*left,Expr*right){};
    Expr(list<Expr*> out_daughters):daughters(out_daughters){};
};

Expr*parse(string input);
void eat_brackets_and_spaces(const string&input,int&s_begin,int&s_end);

class primitive:public Expr{
public:
    static const bool is_primitive=true;
    primitive(char v):Expr(v){};
    primitive(int i):Expr(i){};

};

class unary:public Expr{
public:
    static const bool is_primitive=false;
    const Expr* R=NULL;
    unary(string input){};
};

class binary:public Expr{
public:
    static const bool is_primitive=false;
    binary(string left,string right){};

};

class ternary:public Expr{
public:
    static const bool is_primitive=false;
    const Expr*L=NULL,*R=NULL;
    ternary(string&left, string&rest){
        list<Expr*>::iterator it=daughters.end();
        daughters.insert(it,parse(left));
//        it++;
//        cout<<"chkpt 1 : "<<endl;
//        cout<<s_begin<<" "<<s_end<<" ";
//        cout<<input.substr(sl_begin,sl_end-sl_begin)<<" "<<input.substr(sl_end,s_end-sl_end)<<endl;
        int begin_ptr=0,running_ptr=0,end_ptr=rest.length(),depth=0;
        while(begin_ptr<end_ptr){
            begin_ptr++;running_ptr++;
//            if(begin_ptr<end_ptr){
//
//            }
            if(rest[running_ptr]=='('){
                depth++;
                running_ptr++;
//                cout<<"chkpt 2 : "<<endl;
                while(depth!=0){
                    if(rest[running_ptr]=='('){depth++;}
                    if(rest[running_ptr]==')'){depth--;}
                    running_ptr++;
                }
            }
            else{running_ptr++;}
//            cout<<"chkpt 2.1 : "<<endl;
//            cout<<rest.substr(begin_ptr,running_ptr-begin_ptr)<<endl;
            Expr*curr_deriv=parse(rest.substr(begin_ptr,running_ptr-begin_ptr));
//            cout<<"chkpt 3 : "<<endl;
            if(curr_deriv!=NULL){
                daughters.insert(it,curr_deriv);
//                it++;
            }
            else{
                break;
            }
//            cout<<"chkpt 4 : "<<endl;
//            cout<<rest.substr(running_ptr,end_ptr-running_ptr)<<" yo"<<endl;
            begin_ptr=running_ptr;
//            eat_brackets_and_spaces(rest,begin_ptr,end_ptr);
        }

    };
    ternary(list<Expr*> out_daughters):Expr(out_daughters){
    }
};

class constant:public primitive{
//    Expr::var=0;
public:
    constant(int i):primitive(i){
    };
    void print(){
        cout<<Expr::cons;
    }
    Expr*deriv(char wrt_var){
        return new constant(0);
    }
    Expr*simplify(){}
};

class variable:public primitive{
public:
    variable(char c):primitive(c){
    };
    void print(){
        cout<<Expr::var;
    }
    Expr*deriv(char wrt_var){Expr*out=NULL;
        if(Expr::var==wrt_var){
            return new constant(1);
        }
        else{
            return new constant(0);
        }
    }
    Expr*simplify(){}
};

class logarithm: public unary{
public:
    logarithm(string expression):unary(expression){};
    void print(){}
    Expr*deriv(char wrt_var){}
    Expr*simplify(){}
};

class Div: public binary{
public:
    Div(string left,string right):binary(left,right){}
    void print(){}
    Expr*deriv(char wrt_var){}
    Expr*simplify(){}
};

class Subtr: public binary{
public:
    Subtr(string left,string right):binary(left,right){};
    void print(){}
    Expr*deriv(char wrt_var){}
    Expr*simplify(){}
};

class Expo: public binary{
public:
    Expo(string left,string right):binary(left,right){};
    void print(){}
    Expr*deriv(char wrt_var){}
    Expr*simplify(){}
};

class Addn:public ternary{
public:
    Addn(string left,string rest):ternary(left,rest){};
    Addn(list<Expr*> out_daughters):ternary(out_daughters){};
    void print(){
        list<Expr*>::iterator it=daughters.begin();
        while(it!=daughters.end()){
            (*it)->print();
            it++;
            if(it!=daughters.end())
            cout<<"+";
        }
    }
    Expr*deriv(char wrt_var){
        list<Expr*>out_daughters;
        list<Expr*>::iterator it=daughters.begin();
        list<Expr*>::iterator it2=out_daughters.end();
        while(it!=daughters.end()){
            out_daughters.insert(it2,(*it)->deriv(wrt_var));
            it++;
        }
        cout<<"yo babe"<<endl;
        it=out_daughters.begin();
        while(it!=out_daughters.end()){
            (*it)->print();
            cout<<"  ";
            it++;
        }
        cout<<endl;
        return new Addn(out_daughters);
    }
    Expr*simplify(){}
};

class Multi:public ternary{
public:
    Multi(string left,string rest):ternary(left,rest){};
    void print(){
        list<Expr*>::iterator it=daughters.begin();
        while(it!=daughters.end()){
            (*it)->print();
            it++;
            if(it!=daughters.end())
            cout<<"*";
        }
    }
    Expr*deriv(char wrt_var){}
    Expr*simplify(){}
};

void eat_spaces(const string&input,int&s_begin,int&s_end){
    while(s_begin<s_end){
        if(input[s_begin]==' '){
            s_begin++;
        }
        else if(input[s_end-1]==' '){
            s_end--;
        }
        else break;
    }
    return;
}

void eat_brackets_and_spaces(const string&input,int&s_begin,int&s_end){
    //Eats Extra Spaces/brackets
        eat_spaces(input,s_begin,s_end);

        while(input[s_begin]=='('&&s_begin<s_end){
            s_begin++;
            if(input[s_end-1]!=')'){
                cout<<"Please give a correct input\n";
            }
            s_end--;
            eat_spaces(input,s_begin,s_end);
        }

}


Expr*parse(string input){
    Expr*myExpr;
    int s_end=input.length();
    if(s_end==0){return NULL;}
    int s_begin=0;
    eat_spaces(input,s_begin,s_end);
    if(s_begin>=s_end){
        return NULL;
    }

    if(s_end==s_begin+1){
        if(input[s_begin]>='0'&&input[s_begin]<='9'){
            myExpr=new constant((int)(input[s_begin]-'0'));
        }
        else{
            myExpr=new variable(input[s_begin]);
        }
    }
    else if(input[s_begin]==ln_operator){
        myExpr = new logarithm(input.substr(s_begin+1,s_end-s_begin-1));
    }
    else{
        int sl_begin,sl_end;
        sl_begin=s_begin;
        int depth=0;
        int ptr=s_begin;
        if(input[ptr]=='('){
            depth++;
            ptr++;
            while(depth!=0){
                if(input[ptr]=='('){depth++;}
                if(input[ptr]==')'){depth--;}
                ptr++;
            }
        }
        else{ptr++;}
        sl_end=ptr;
        switch(input[sl_end]){
            case '+':
                cout<<"chkpt 0 : "<<s_begin<<" "<<s_end<<" "<<input.substr(sl_begin,sl_end-sl_begin)<<" "<<input.substr(sl_end,s_end-sl_end)<<endl;
                myExpr=new Addn(input.substr(sl_begin,sl_end-sl_begin),input.substr(sl_end,s_end-sl_end));
                break;
            case '*':
                myExpr=new Multi(input.substr(sl_begin,sl_end-sl_begin),input.substr(sl_end,s_end-sl_end));
                break;
            case '-':
                myExpr=new Subtr(input.substr(sl_begin,sl_end-sl_begin),input.substr(sl_end,s_end-sl_end));
                break;
            case '^':
                myExpr=new Expo(input.substr(sl_begin,sl_end-sl_begin),input.substr(sl_end,s_end-sl_end));
                break;
        }
    }
    return myExpr;
}

int main(){
    int T;
    string input;
    cout<<"Enter Number of Test Cases: ";
    scanf("%d",&T);
    //string asdf;getline(cin,asdf);
    //cout<<asdf<<endl;
    T+=1;
    int i=T;
    while(T-->0)
    {
        if(i!=T+1)
            cout<<"\nEnter a prefix Expression with parenthesis with/without spaces:\n";
        getline(cin,input);
        Expr*myExpr=parse(input);
        if(myExpr!=NULL){
            myExpr->print();
        }
        Expr*der_myExpr=NULL;
        cout<<"printed Successfully"<<endl;
        if(myExpr!=NULL){
            der_myExpr=myExpr->deriv('x');
        }
        if(der_myExpr!=NULL){
            cout<<"Derivative of Entered Expression:\n";
            der_myExpr->print();
        }
        if(i!=T+1)
            cout<<endl<<endl<<endl;

    }

    return 0;
}
