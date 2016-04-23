///Thid program tries finding multiple roots for the operators stated below:
///Runs newton Rhapsody for different initial values in range.(in steps of 0.1)

///## This is now parsing as an INFIX expression
///## The different classes are more organized(Easily Extendable to include more functions)
///## The derivative function now takes a parameter

///##  This function works with the following binary operators:
///##  + : Addition         (NEW - SEE NOTE)
///##  - : Subtraction
///##  * : Multiplication
///##  ^ : Exponentiation   (NEW- variable may appear in the exponent)
///##  # : Logarithm        (NEW)
///##  / : Division         (NEW)
///##  Please ensure that the variable names/numbers entered are only one character long!
///NOTE: Addition has been implemented as TERNARY Operator(Not Binary)

#include<iostream>
#include<string.h>
#include<stdio.h>
#include<math.h>
#include<list>
using namespace std;
//using namespace list::std;
#define ln_operator '#'

class Expr{
protected:
    bool is_primitive;
    char var;
    int cons;
    Expr* L,*R;
    list<Expr*> daughters;
public:
    virtual void print()=0;
    virtual Expr*deriv(char wrt_var)=0;
    virtual Expr*simplify()=0;
    virtual float evaluate(float x)=0;
    bool find_roots(float x,float y);
protected:
    Expr(){};
    Expr(bool b){};
    Expr(char v):var(v){};
    Expr(int i):cons(i){};
    Expr(string input){};
    Expr(Expr*left,Expr*right):L(left),R(right){};
    Expr(Expr*left):L(left){};
    Expr(list<Expr*> out_daughters):daughters(out_daughters){};
};

Expr*parse(string input);
void eat_brackets_and_spaces(const string&input,int&s_begin,int&s_end);

class primitive:public Expr{
protected:
    static const bool is_primitive=true;
    primitive(char v):Expr(v){};
    primitive(int i):Expr(i){};

};

class unary:public Expr{
protected:
    static const bool is_primitive=false;
    const Expr* R=NULL;
    unary(string input){
        L=parse(input);
    };
    unary(Expr* input):Expr(input){};
};

class binary:public Expr{
protected:
    static const bool is_primitive=false;
    void print(char c){
        cout<<"(";
        L->print();
        cout<<c;
        R->print();
        cout<<")";
    }
    binary(string left,string right){
        L=parse(left);
        R=parse(right);
    };
    binary(Expr* left,Expr* right):Expr(left,right){};

};

class ternary:public Expr{
protected:
    static const bool is_primitive=false;
    const Expr*L=NULL,*R=NULL;
    void print(char c){
        list<Expr*>::iterator it=daughters.begin();
        cout<<"(";
        while(it!=daughters.end()){
            (*it)->print();
            it++;
            if(it!=daughters.end())
            cout<<c;
        }
        cout<<")";
    }
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
    ternary(list<Expr*> out_daughters):Expr(out_daughters){}
    ternary(Expr* left,Expr* right){
        list<Expr*>::iterator it=daughters.end();
        daughters.insert(it,left);
        daughters.insert(it,right);
    }
};

class constant:public primitive{
public:
    constant(int i);
    void print();
    Expr*deriv(char wrt_var);
    Expr*simplify();
    float evaluate(float x);
};

class variable:public primitive{
public:
    variable(char c);
    void print();
    Expr*deriv(char wrt_var);
    Expr*simplify();
    float evaluate(float x);
};

class logarithm: public unary{
public:
    logarithm(string expression);
    logarithm(Expr* expression);
    void print();
    Expr*deriv(char wrt_var);
    Expr*simplify();
    float evaluate(float x);
};

class Div: public binary{
public:
    Div(string left,string right);
    Div(Expr* left,Expr* right);
    void print();
    Expr*deriv(char wrt_var);
    Expr*simplify();
    float evaluate(float x);
};

class Substr: public binary{
public:
    Substr(string left,string right);
    Substr(Expr* left,Expr* right);
    void print();
    Expr*deriv(char wrt_var);
    Expr*simplify();
    float evaluate(float x);
};

class Expo: public binary{
public:
    Expo(string left,string right);
    Expo(Expr* left,Expr* right);
    void print();
    Expr*deriv(char wrt_var);
    Expr*simplify();
    float evaluate(float x);
};

class Addn:public ternary{
public:
    Addn(string left,string rest);
    Addn(list<Expr*> out_daughters);
    Addn(Expr* left,Expr* right);
    void print();
    Expr*deriv(char wrt_var);
    Expr*simplify();
    float evaluate(float x);
};

class Multi:public binary{
public:
    Multi(string left,string rest);
//    Multi(list<Expr*> out_daughters);
    Multi(Expr* left,Expr* right);
    void print();
    Expr*deriv(char wrt_var);
    Expr*simplify();
    float evaluate(float x);
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
        return myExpr;

    }
    else{
        s_begin++;s_end--;
    }
    if(input[s_begin]==ln_operator){
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
//                cout<<"chkpt 0 : "<<s_begin<<" "<<s_end<<" "<<input.substr(sl_begin,sl_end-sl_begin)<<" "<<input.substr(sl_end,s_end-sl_end)<<endl;
                myExpr=new Addn(input.substr(sl_begin,sl_end-sl_begin),input.substr(sl_end,s_end-sl_end));
                break;
            case '*':
                myExpr=new Multi(input.substr(sl_begin,sl_end-sl_begin),input.substr(sl_end+1,s_end-sl_end-1));
                break;
            case '-':
                myExpr=new Substr(input.substr(sl_begin,sl_end-sl_begin),input.substr(sl_end+1,s_end-sl_end-1));
                break;
            case '^':
                myExpr=new Expo(input.substr(sl_begin,sl_end-sl_begin),input.substr(sl_end+1,s_end-sl_end-1));
                break;
            case '/':
                myExpr=new Div(input.substr(sl_begin,sl_end-sl_begin),input.substr(sl_end+1,s_end-sl_end-1));
                break;
            default:
                return NULL;

        }
    }
    return myExpr;
}

bool found(list<float> all_roots, float f){
    list<float>::iterator i1=all_roots.begin();
    while(i1!=all_roots.end()){
        if((*i1)==f){
            return true;
        }
        i1++;
    }
    return false;
}


//Expr
bool Expr::find_roots(float x,float y){

    Expr*my_deriv=this->deriv('x');
    list<float> all_roots;
//    my_deriv->print();
//    cout<<endl;
//    cout<<x<<" "<<y<<endl;
    while(x<y){
        bool found_root=true;
        float x_var=x;
        float deriv_eval=my_deriv->evaluate(x_var);
        float func_eval=this->evaluate(x_var);
        if(func_eval==0){
        }
        else{
            float x_prev;
        //    cout<<"x: "<<x<<" f: "<<func_eval<<" f': "<<deriv_eval<<endl;
            int i=0;
            do{
                if(deriv_eval!=0&&i<100000){
                    x_prev=x_var;
                    x_var=x_var-func_eval/deriv_eval;
                    deriv_eval=my_deriv->evaluate(x_var);
                    func_eval=evaluate(x_var);
        //            cout<<"x: "<<x<<" f: "<<func_eval<<" f': "<<deriv_eval<<endl;
                }
                else{
                    found_root=false;
                    break;
                }
                i++;
            }while(x_var!=x_prev);
        }

        if(found_root){
//            cout<<"found a root"<<endl;
            if(!found(all_roots,x_var)){
                all_roots.insert(all_roots.begin(),x_var);
                cout<<"Root: "<<x_var<<endl;
            }
        }
        x+=0.1;
    }

    if(all_roots.empty()){
        cout<<"Couldnt find any roots in given range"<<endl;
        return false;
    }
    else{
        return true;
    }
}


//constant:
constant::constant(int i):primitive(i){};
void constant::print(){
    cout<<Expr::cons;
}
Expr*constant::deriv(char wrt_var){
    return new constant(0);
}
Expr*constant::simplify(){};
float constant::evaluate(float x){
    return cons;
}


//variable:
variable::variable(char c):primitive(c){};
void variable::print(){
    cout<<Expr::var;
}
Expr*variable::deriv(char wrt_var){
    if(Expr::var==wrt_var){
        return new constant(1);
    }
    else{
        return new constant(0);
    }
}
Expr*variable::simplify(){}
float variable::evaluate(float x){
    return x;
}

//logarithm:
logarithm::logarithm(string expression):unary(expression){};
logarithm::logarithm(Expr* expression):unary(expression){};
void logarithm::print(){
    cout<<"(log ";
    L->print();
    cout<<")";
}
Expr*logarithm::deriv(char wrt_var){
    Expr*l_deriv= L->deriv(wrt_var);
    Expr*out=new Div(l_deriv,L);
}
Expr*logarithm::simplify(){}
float logarithm::evaluate(float x){
    float f= L->evaluate(x);
    if(f<=0){

    }
    else{
        return log(f);
    }

}

//Div:
Div::Div(string left,string right):binary(left,right){}
Div::Div(Expr* left,Expr* right):binary(left,right){}
void Div::print(){
    binary::print('/');
}
Expr*Div::deriv(char wrt_var){
    Expr*l_deriv=L->deriv(wrt_var);
    Expr*r_deriv=R->deriv(wrt_var);
    Expr*numerator=new Substr(new Multi(l_deriv,R),new Multi(L,r_deriv));
    Expr*denominator=new Expo(R,new constant(2));
    Expr*out=new Div(numerator,denominator);
    return out;
}
Expr*Div::simplify(){}
float Div::evaluate(float x){
    float f= R->evaluate(x);
    if(f==0){

    }
    else{
        return (L->evaluate(x))/f;
    }

}

//Substr:
Substr::Substr(string left,string right):binary(left,right){};
Substr::Substr(Expr* left,Expr* right):binary(left,right){}
void Substr::print(){
    binary::print('-');
}
Expr*Substr::deriv(char wrt_var){
    Expr*out=new Substr(L->deriv(wrt_var),R->deriv(wrt_var));
    return out;
}
Expr*Substr::simplify(){}
float Substr::evaluate(float x){
    return L->evaluate(x)-R->evaluate(x);

}

//Expo:
Expo::Expo(string left,string right):binary(left,right){};
Expo::Expo(Expr* left,Expr* right):binary(left,right){}
void Expo::print(){
    binary::print('^');
}
Expr*Expo::deriv(char wrt_var){
    Expr*ln_left=new logarithm(L);
    Expr*r_deriv=R->deriv(wrt_var);
    Expr*l_deriv=L->deriv(wrt_var);
    Expr*term1=new Multi(R,new Div(l_deriv,L));
    Expr*term2=new Multi(r_deriv,ln_left);
    Expr*full_term=new Addn(term1,term2);
    Expr*this_expr=this;
    Expr*out=new Multi(this_expr,full_term);
    return out;
}
Expr*Expo::simplify(){}
float Expo::evaluate(float x){
    return pow(L->evaluate(x),R->evaluate(x));
}

//Addn:
Addn::Addn(string left,string rest):ternary(left,rest){};
Addn::Addn(list<Expr*> out_daughters):ternary(out_daughters){};
Addn::Addn(Expr* left,Expr* right):ternary(left,right){};
void Addn::print(){
    ternary::print('+');
}
Expr*Addn::deriv(char wrt_var){
    list<Expr*>out_daughters;
    list<Expr*>::iterator it=daughters.begin();
    list<Expr*>::iterator it2=out_daughters.end();
    while(it!=daughters.end()){
        out_daughters.insert(it2,(*it)->deriv(wrt_var));
        it++;
    }
//    cout<<"yo babe"<<endl;
//    it=out_daughters.begin();
//    while(it!=out_daughters.end()){
//        (*it)->print();
//        cout<<"  ";
//        it++;
//    }
    cout<<endl;
    return new Addn(out_daughters);
}
Expr*Addn::simplify(){
}
float Addn::evaluate(float x){
    float f=0;
//    cout<<"x:"<<x<<endl;
    list<Expr*>::iterator i1=daughters.begin();
    while(i1!=daughters.end()){
        f+=(*i1)->evaluate(x);
//        cout<<"yo"<<endl;
        i1++;
    }
    return f;
}

//Multi:
Multi::Multi(string left,string rest):binary(left,rest){};
//Multi::Multi(list<Expr*> out_daughters):binary(out_daughters){};
Multi::Multi(Expr* left,Expr* right):binary(left,right){};
void Multi::print(){
    binary::print('*');
}
Expr*Multi::deriv(char wrt_var){
    Expr*l_deriv=L->deriv(wrt_var);
    Expr*r_deriv=R->deriv(wrt_var);
    Expr*term1=new Multi(l_deriv,R);
    Expr*term2=new Multi(L,r_deriv);
    Expr*out=new Addn(term1,term2);
    return out;
}
Expr*Multi::simplify(){}
float Multi::evaluate(float x){
    return (L->evaluate(x))*(R->evaluate(x));
}


int main(){
    int T;
    string input;
    cout<<"Enter Number of Test Cases: ";
    scanf("%d",&T);
    int i=T;
    while(T-->0)
    {
        cout<<"\nEnter an infix Expression(in x) with parenthesis without spaces:\n";
        cin>>input;
        float x1,y1;
        cout<<"Enter the range of the desired roots:\n";
        cin>>x1>>y1;

        Expr*myExpr=parse(input);
        if(myExpr!=NULL){
            myExpr->find_roots(x1,y1);
        }
        else{
            cout<<"Could not parse";
        }
//        Expr*der_myExpr=NULL;
//        cout<<"printed Successfully"<<endl;
//        if(myExpr!=NULL){
//            der_myExpr=myExpr->deriv(c);
//        }
//        if(der_myExpr!=NULL){
//            cout<<"Derivative of Entered Expression:\n";
//            der_myExpr->print();
//        }
        cout<<endl;

    }

    return 0;
}
