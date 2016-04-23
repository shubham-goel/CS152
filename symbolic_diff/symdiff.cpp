///##  This function works with the following binary operators:
///##  + : Addition
///##  - : Subtraction
///##  * : Multiplication
///##  ^ Exponentiation
///##  Please ensure that the variable names/numbers entered are only one character long!
///##****   I have modified the class to perform basic algebraic manipulations on numbers entered. *******
///##****   (SEE TEST CASE 1 AS EXAMPLE) *******

#include<iostream>
#include<string.h>
#include<stdio.h>
#include<math.h>
using namespace std;

class Expr{

private:
    bool is_primitive;
    bool is_num;
    char val;
    int val1;
    char op;
    Expr* L,*R;

    ///PRIVATE UTILITY FUNCTIONS:
private:
    void trans_to_L(){
        is_primitive=L->is_primitive;
        is_num      =L->is_num;
        val         =L->val;
        val1        =L->val1;
        op          =L->op;
        R           =L->R;
        Expr* l     =L->L;
        L           =l;
    }
    void trans_to_R(){
        is_primitive=R->is_primitive;
        is_num      =R->is_num;
        val         =R->val;
        val1        =R->val1;
        op          =R->op;
        L           =R->L;
        Expr* r     =R->R;
        R           =r;
    }
public:
    void del_expr(){
        if(!is_primitive){
            if(L->is_primitive){
                delete L;
            }
            else{
                L->del_expr();
                delete L;
            }
            if(R->is_primitive){
                delete R;
            }
            else{
                R->del_expr();
                delete R;
            }

        }
    }

    ///PUBLIC MEMBER FUNCTIONS:
public:
    void print(){
        if(is_primitive){
            if(is_num){
                if(val1>=0){
                    cout<<val1;
                }
                else{
                    cout<<"("<<val1<<")";
                }
            }
            else{
                cout<<val;
            }

        }
        else{
            cout<<"(";
            if(!(L->is_primitive&&L->is_num&&L->val1==0))
                L->print();
            cout<<op;
            R->print();
            cout<<")";
        }
    }

    Expr* deriv(){
        Expr* out=NULL;


        if(is_primitive){
            if(val=='x'){
                out = new Expr(1);
            }
            else{
                out = new Expr(0);
            }
        }
        else{
            Expr*l,*r;


            l=L->deriv();
            r=R->deriv();
            if(!(l==NULL||r==NULL))
            switch(op){
                case '+':
                    out=new Expr('+',l,r);
                    break;
                case '-':
                    out=new Expr('-',l,r);
                    break;
                case '^':
                    if(!(r->is_primitive&&r->is_num&&r->val1==0)){
                        cout<<"Input Not Supported! Check Exponentiation!\n";
                        out = NULL;
                    }
                    else if(R->is_primitive&&R->is_num&&R->val1==0){
                        out = new Expr(0);
                    }
                    else if(l->is_primitive&&l->is_num&&l->val1==0){
                        out = new Expr(0);
                    }
                    else {
                        Expr*one=new Expr(1);
                        Expr*R_minus1=new Expr('-',R,one);
                        Expr*LR_minus1= new Expr('^',L,R_minus1);
                        Expr*Rl = new Expr('*',R,l);
                        out=new Expr('*',Rl,LR_minus1);

                    }
                    break;

                case '*':
                    Expr*l1=new Expr('*',l,R);
                    Expr*r1=new Expr('*',L,r);
                    out=new Expr('+',l1,r1);
                    break;
            }
        }
        return out;
    }

    ///CONSTRUCTORS:
public:
    Expr(){
        is_primitive=true;
        is_num=true;
        val=0;
        val1=0;
        L=NULL;
        R=NULL;
    }
    Expr(char v){
        is_primitive=true;
        if(v>='0'&&v<='9'){
            is_num=true;
            val=0;
            val1=v-'0';
        }
        else{
            is_num=false;
            val=v;
            val1=0;
        }
        L=NULL;
        R=NULL;
    }
    Expr(int v){
        is_primitive=true;
        is_num=true;
        val=0;
        val1=v;
        L=NULL;
        R=NULL;
    }
    Expr(char opr, Expr*left, Expr*right){
        is_primitive=false;
        is_num=false;
        val=0;
        val1=0;
        op=opr;
        L=left;
        R=right;
        if(L->is_primitive||R->is_primitive)
        switch(op){
            case '+':
                if(L->is_primitive&&L->is_num&&L->val1==0){
                    trans_to_R();
                }
                else if(R->is_primitive&&R->is_num&&R->val1==0){
                    trans_to_L();
                }
                else if(L->is_primitive&&L->is_num&&R->is_primitive&&R->is_num){
                    is_primitive=true;
                    is_num=true;
                    val=0;
                    val1=L->val1+R->val1;
                    L=NULL;
                    R=NULL;
                }
                break;
            case '*':
                if((L->is_primitive&&L->is_num&&L->val1==0)||(R->is_primitive&&R->is_num&&R->val1==0)){
                    is_primitive=true;
                    is_num=true;
                    val=0;
                    val1=0;
                    L=NULL;
                    R=NULL;
                }
                else if(L->is_primitive&&L->is_num&&L->val1==1){
                    trans_to_R();
                }
                else if(R->is_primitive&&R->is_num&&R->val1==1){
                    trans_to_L();
                }
                else if(L->is_primitive&&L->is_num&&R->is_primitive&&R->is_num){
                    is_primitive=true;
                    is_num=true;
                    val=0;
                    val1=L->val1*R->val1;
                    L=NULL;
                    R=NULL;
                }
                break;
            case '-':
                if(L->is_primitive&&L->is_num&&R->is_primitive&&R->is_num){
                    is_primitive=true;
                    is_num=true;
                    val=0;
                    val1=L->val1-R->val1;
                    L=NULL;
                    R=NULL;
                }
                else if(R->is_primitive&&R->is_num&&R->val1==0){
                    trans_to_L();
                }
                break;
            case '^':
                if(L->is_primitive&&L->is_num&&L->val1==0){
                    is_primitive=true;
                    is_num=true;
                    val1=0;
                }
                else if(R->is_primitive&&R->is_num&&R->val1==0){
                    is_primitive=true;
                    is_num=true;
                    val1=1;
                }
                else if(L->is_primitive&&L->is_num&&L->val1==1){
                    is_primitive=true;
                    is_num=true;
                    val1=1;
                }
                else if(R->is_primitive&&R->is_num&&R->val1==1){
                    trans_to_L();
                }
                else if(L->is_primitive&&L->is_num&&R->is_primitive&&R->is_num){
                    is_primitive=true;
                    is_num=true;
                    val=0;
                    val1=(int)pow(L->val1,R->val1);
                    L=NULL;
                    R=NULL;
                }
                break;
        }
        else{

        }
        if(!is_primitive&&(L==NULL||R==NULL)){
            cout<<"NULL PROBLEM\n";
        }
    }

    ~Expr(){
        del_expr();
    }

};

void eat_spaces(string input,int&s_begin,int&s_end){
    while(true){
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

Expr* parse(string input){
    Expr*myExpr;
    int s_end=input.length();
    if(s_end==0){return NULL;}
    int s_begin=0;
    eat_spaces(input,s_begin,s_end);

    while(input[s_begin]=='('){
        s_begin++;
        if(input[s_end-1]!=')'){
            cout<<"Please give a correct input\n";
            return NULL;
        }
        s_end--;
        eat_spaces(input,s_begin,s_end);
    }

    if(s_end==s_begin+1){
        myExpr=new Expr(input[s_begin]);
        return myExpr;
    }

    int depth=0;
    int ptr=s_begin+1;
    int sl_begin,sl_end;
    while(input[ptr]==' '){
        ptr++;
    }
    sl_begin=ptr;
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
    string lhs=input.substr(sl_begin,sl_end-sl_begin);
    while(input[ptr]==' '){
        ptr++;
    }
    string rhs=input.substr(ptr,s_end-ptr);
    Expr*L=parse(lhs);
    Expr*R=parse(rhs);

    switch(input[s_begin]){
        case '+':
            myExpr=new Expr('+',L,R);
            break;
        case '*':
            myExpr=new Expr('*',L,R);
            break;
        case '-':
            myExpr=new Expr('-',L,R);
            break;
        case '^':
            myExpr=new Expr('^',L,R);
            break;
    }
    return myExpr;
}



int main(){
    cout<<"\
    ##  This function works with the following binary operators:\n\
    ##  + : Addition\n\
    ##  - : Subtraction\n\
    ##  * : Multiplication\n\
    ##  ^ Exponentiation\n\
    ##\n\
    ##  Please ensure that the variable names/numbers entered\n\
    ##  are only one character long!\n\
    \n\
    ";

    int T;
    string input;
    cout<<"Enter Number of Test Cases: ";
    scanf("%d",&T);
    T+=1;
    int i=T;
    while(T-->0)
    {
        if(i!=T+1)
            cout<<"\nEnter a prefix Expression with parenthesis with/without spaces:\n";
        getline(cin,input);
        Expr*myExpr=parse(input);
//        myExpr->print();
        Expr*der_myExpr=NULL;
        cout<<endl;
        if(myExpr!=NULL){
            der_myExpr=myExpr->deriv();
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
