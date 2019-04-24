using namespace std;

int main(){
    stack <int> s;
    char exp[]="245+-*5+";
    for(int i=0;i<8;i++){
        if(exp[i]>=48 && exp[i]<=57 ){
            s.push(exp[i]);
        }
        else if(exp[i]=='+' || exp[i]=='-' || exp[i]=='*' || exp[i]=='/'){
            int op1=s.top();
            s.pop();
            int op2=s.top();
            s.pop();
            if(exp[i]=='+')
                s.push(op2+op1);
            else if(exp[i]=='-')
                 s.push(op2-op1);
            else if(exp[i]=='-')
                 s.push(op2-op1);
            else if(exp[i]=='*')
                 s.push(op2*op1);
            else if(exp[i]=='/')
                 s.push(op2/op1);
        }
    }
    int result=s.top();
    s.pop();
    cout<<"result"<<result;
}
