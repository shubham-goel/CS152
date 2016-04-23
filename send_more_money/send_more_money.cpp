/// This program takes as input any 3 strings and returns all solutions
/// to A,B,C such that A + B == C
/// The user is asked if he wants distinct values or not
/// The first digit of a number may be 0

#include<iostream>
#include<map>
#include<string.h>
using namespace std;

class syst{
    private:
        int*pos;            /// pos STORES VALUES OF ALL VARIABLES
        map<char,int*> m;   /// m MAPS CHAR TO AN ADDRESS IN pos
        string A;
        string B;
        string C;
                            /// A,B,C => INPUT STRINGS REVERSED, SPACES APPENDED TO THEM TO MAKE OF EQUAL LENGTH (REDUCES COMPLEXITY)
        bool* carry_forward;/// carry_forward IS TRUE IF 1 IS CARRIED
        bool need_unique;   /// need_unique SHOULD VALUES BE DISTINCT?
        int num_variables;  /// num_variables => NUMBER OF VARIABLES
        int number_size;    /// number_size => LENGTH OF A,B,C
        int depth;          /// depth :: VARIANT >=0,<=num_variables IS USED FOR PARTIAL SOLNS/BACKTRACKING IN pos
        int depth1;         /// depth1 :: VARIANT >=0,<=number_size IS USED FOR PARTIAL SOLNS/BACKTRACKING IN A,B,C
        int*depth_map;      /// depth_map MAPS depth1 TO depth. depth_map[i]=NUMBER OF DISTINCT VARIABLES IN FIRST i COLUMNS
        int zero=0;         /// zero IS A MEMORY LOCATION CONTAINING 0.

    public:
        void print(){/// PRINTS SYSTEM
            // Spaces were appended. Dont map to an int variable;
            // Prints in Succession A,B,C as (char):(value)
            cout<<"\n ";
            for(int i=number_size-1;i>=0;i--){
                if(A[i]==' '){
                    cout<<"    ";// Aligns Numbers vertically
                }
                else{
                    cout<<A[i]<<":"<<*m[A[i]]<<" ";
                }
            }
            cout<<"\n+";
            for(int i=number_size-1;i>=0;i--){
                if(B[i]==' '){
                    cout<<"    ";
                }
                else{
                    cout<<B[i]<<":"<<*m[B[i]]<<" ";
                }
            }
            cout<<"\n-";
            for(int i=number_size-1;i>=0;i--){
                cout<<"----";
            }
            cout<<"\n=";
            for(int i=number_size-1;i>=0;i--){
                if(C[i]==' '){
                    cout<<"    ";
                }
                else{
                    cout<<C[i]<<":"<<*m[C[i]]<<" ";
                }
            }
            cout<<endl;
        }

        bool are_distinct(){/// CHECKS IF pos[depth-1] IS DISTINCT;
            for(int i=0;i<depth-1;i++){
                if(pos[i]==pos[depth-1]){return false;}
            }
            return true;
        }

        bool valid(){/// CHECKS VALIDITY OF PARTIAL EQUATION
            if(need_unique){// Distinctness checked if unique solns needed
                if(!are_distinct()){
                    return false;
                }
            }
            if(!(depth_map[depth1]==depth)){// Column depth1 contains unassigned characters => return true
                return true;
            }

            if(depth1==0){// Empty Board
                return true;
            }
            else{
                int char_A=*m[A[depth1-1]];
                int char_B=*m[B[depth1-1]];
                int char_C=*m[C[depth1-1]];
                if((char_A+char_B+((carry_forward[depth1-1])?1:0))%10!=char_C){// invalid if (sum of 2 digits + carry) does not give char_C?
                    return false;
                }
                else{
                    if(depth1==number_size){// check specially if whole board is passed(there should be no carry to the next col)
                        if(((char_A+char_B+((carry_forward[depth1-1])?1:0))<=9)){
                            return true;
                        }
                        else{
                            return false;
                        }
                    }
                    else{
                        return true;
                    }
                }

            }

        }
        void extend(){/// EXTENDS BOARD(PARTIAL SOLUTION) TO INCLUDE NEW COLUMNS
            if(depth1>0){// Update carry_forward
                carry_forward[depth1]=((*m[A[depth1-1]]+*m[B[depth1-1]]+((carry_forward[depth1-1])?1:0))>=10)?true:false;
            }
            else{
                carry_forward[depth1]=false;
            }
            if(depth_map[depth1]==depth){// last column is completely filled
                    depth1++;// Increments column
            }
            if(depth<depth_map[depth1]){// New column has unassigned variables =>assigns them
                    pos[depth]=0;
                    depth++;
            }
        }
        bool backtrack(){/// BACKTRACKS pos ARRAY (similar to 8 queens)
            while(true){
                if(pos[depth-1]<9){// if last variable can be incremented
                    pos[depth-1]++;// increment
                    break;
                }else{// keep going to previous variable
                    pos[depth-1]=-1;// initialize unassigned variable
                    depth--;
                    if(depth==0){// covered all cases => found all solutions
                        return false;
                    }
                }
            }
            {// Set depth1 according to depth, move depth1 to column containing last variable
                depth1=0;
                while(depth_map[depth1]<depth){
                    depth1++;
                }
                return true;
            }
        }
        bool full(){/// SELF-EXPLANATORY
            if(depth1==number_size&&depth==num_variables){// Board can no longer be extended
                return true;
            }
            else{
                return false;
            }
        }
        syst(string x,string y,string z,bool b){/// CONSTRUCTOR
            need_unique=b;// set need_unique

            // Initialize depth,depth1 to 0
            depth=0;
            depth1=0;
            number_size=maximum(x.length(),y.length(),z.length());// Calculate desired Length of A,B,C

            {// Set Strings A,B,C
                A=reverse_str(x);// Reverse
                B=reverse_str(y);
                C=reverse_str(z);
                make_of_length(A,number_size);// Append.
                make_of_length(B,number_size);
                make_of_length(C,number_size);
            }

            // Assign carry_forward,depth_map and check for NULL ptrs
            carry_forward=new bool[number_size];
            depth_map=new int[number_size+1];
            if(carry_forward==NULL){
                cout<<"Allocation carry_forward"<<endl;
            }
            if(depth_map==NULL){
                cout<<"Allocation depth_map"<<endl;
            }

            {// Counting num_variables for assigning pos
                num_variables=0;
                map<char,char> temp_map;// temporary map
                for(int i=0;i<number_size;i++){
                    if(temp_map.count(A[i])==0&&A[i]!=' '){// if char is not present and is not ' '
                        temp_map[A[i]]=0;// make it present
                        num_variables++;// increment num_variables
                    }
                    if(temp_map.count(B[i])==0&&B[i]!=' '){
                        temp_map[B[i]]=0;
                        num_variables++;
                    }
                    if(temp_map.count(C[i])==0&&C[i]!=' '){
                        temp_map[C[i]]=0;
                        num_variables++;
                    }
                }
            }

            // Assign pos and check for NULL ptrs
            pos=new int[num_variables];
            if(pos==NULL){
                cout<<"Allocation pos"<<endl;
            }

            // Setting depth_map,m
            num_variables=0;
            depth_map[0]=0;// Always true
            for(int i=0;i<number_size;i++){
                if(m.count(A[i])==0&&A[i]!=' '){// if char is not present and is not ' '
                    m[A[i]]=&pos[num_variables];// map (num_variables+1)th variable to pos[num_variables];make it present in m
                    num_variables++;// increment num_variables
                }
                if(m.count(B[i])==0&&B[i]!=' '){
                    m[B[i]]=&pos[num_variables];
                    num_variables++;
                }
                if(m.count(C[i])==0&&C[i]!=' '){
                    m[C[i]]=&pos[num_variables];
                    num_variables++;
                }
                depth_map[i+1]=num_variables;// num_variables=number of distinc variables in first i columns
            }

            // initialize pos,carry_forward
            for(int i=0;i<num_variables;i++){
                pos[i]=-1;
                if(i<number_size){
                    carry_forward[i]=false;
                }
            }

            // Appended spaces always map to 0;
            m[' ']=&zero;

            {// Was used for debugging.
//             cout<<"numvar: "<<num_variables<<" numsize:"<<number_size<<endl;
//             cout<<depth<<" "<<depth1<<endl;
//             print();
//             cout<<endl<<"Depth_map: {";
//             for(int i=0;i<number_size;i++){
//                 cout<<depth_map[i]<<",";
//             }
//             cout<<depth_map[number_size]<<"}"<<endl;
//             cout<<"A: {"<<A<<"}  B: {"<<B<<"}  C: {"<<C<<"}"<<endl<<"carry forward: ";
//             for(int i=0;i<number_size;i++){
//                 cout<<carry_forward[i]<<",";
//             }
//             cout<<"numvar: "<<num_variables<<" numsize:"<<number_size<<endl;
//             cout<<"\nConstructor Ended\n";gu
            }
        }
        string reverse_str(string&x){/// RETURNS REVERSED STRING
            int len_str=x.length();
            int i;
            string y;
            for(i=0;i<len_str;i++){
                if(i==0){// Assign first element of y
                    y.assign(&x[len_str-1-i]);
                }
                else{// Append the rest
                    y+=x[len_str-1-i];
                }
            }
            return y;
        }
        int maximum(int x, int y){/// RETURNS max VALUE
            if(x>y){
                return x;
            }
            else return y;
        }
        int maximum(int x,int y, int z){/// maximum OVERLOADED FOR 3 ARGUMENTS
            return maximum(maximum(x,y),z);
        }
        void make_of_length(string&x,int len){/// APPENDS SPACES TO STRING x
            int i=x.length();
            while(i<len){
                x+=' ';
                i++;
            }
            return;
        }
};


int main(){
    bool notDone;
    cout<<"ho!"<<endl;
    string A,B,C;
    cout<<"Enter String A: ";cin>>A;
    cout<<"Enter String B: ";cin>>B;
    cout<<"Enter String C: ";cin>>C;
    cout<<"Solving for A+B=C:\nShould digits be assigned unique integers?(Y/N)\t";
    // Temporariy using notDone
    char b;
    cin>>b;
    if(b=='Y'){
        notDone=true;
    }
    else if(b=='N'){
        notDone=false;
    }
    else{
        cout<<"Invalid Input"<<endl;
        return 0;
    }

    // INITIALIZATION
    syst mysyst(A,B,C,notDone);
    notDone=true;
    int num=0;

    while(notDone){
        if(mysyst.valid()){
            if(mysyst.full()){// Full Solution found
                mysyst.print();
                cout<<endl;
                num++;
                if(!mysyst.backtrack()){notDone=false;}
            }else{// Partial Solution Found
                mysyst.extend();
            }
        }else{// Invalid Solution
            if(!mysyst.backtrack()){notDone=false;}
        }
    }
    cout<<"Total Solutions: "<<num<<endl;
    return 0;
}
