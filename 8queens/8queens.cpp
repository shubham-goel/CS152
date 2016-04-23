#include<iostream>
#include<math.h>
using namespace std;

///PLEASE ENTER THE VALUE OF N HERE:
#define N 8

class Board{
    private:
        int pos[N];
        int depth;
    public:
        void print(){
            for(int i=0;i<N;i++){
                cout<<pos[i]<<" ";
            }
            cout<<endl;
        }
        bool valid(){
            for(int i=0;i<depth-1;i++){
                if(pos[i]==pos[depth-1]){return false;}
                if(fabs(pos[i]-pos[depth-1])==fabs(i-depth+1)){return false;}
            }
            return true;
        }
        void extend(){
            pos[depth]=0;
            depth++;
        }
        bool backtrack(){
            while(true){
                if(pos[depth-1]<N-1){
                    pos[depth-1]++; break;
                }else{
                    depth--;
                    if(depth==0){return false;}
                }
            }
            return true;
        }
        bool full(){
            if(depth==N){return true;}
            else{return false;}
        }
        Board(){
            for(int i=0;i<N;i++){
                pos[i]=-1;
            }
            depth=0;
        }
};

int main(){

    bool notDone=true;
    Board myBoard;
    int num=0;

    while(notDone){
        if(myBoard.valid()){
            if(myBoard.full()){
               // if(num==0){cout<<"First Solution: ";myBoard.print();}
                myBoard.print();
                num++;
                if(!myBoard.backtrack()){notDone=false;}
            }else{
                myBoard.extend();
            }
        }else{
            if(!myBoard.backtrack()){notDone=false;}
        }
    }
    cout<<"Total Solutions: "<<num<<endl;
    return 0;
}
