#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;

bool legal(char curr, char prev);
bool validinfix(string test);
string translate(string newinfix);
int precedence(char op);

int evaluate(string infix, string& postfix, bool& result)
{
    string nsinfix = "";
    
    for(int i=0; i<infix.length(); i++)
    {
        if(infix[i]!=' ')
            nsinfix+= infix[i];
    }
    
    bool b = validinfix(nsinfix);
    
    if(!b)
        return(1);
    
    postfix = translate(nsinfix);
    
    stack<bool> eval_Stack;
    
    for(int i=0; i<postfix.length(); i++)
    {
        if(postfix[i] == 'T')
            eval_Stack.push(true);
        else if(postfix[i] == 'F')
            eval_Stack.push(false);
        else if(postfix[i] == '&')
        {
            bool b1 = eval_Stack.top();
            eval_Stack.pop();
            bool b2 = eval_Stack.top();
            eval_Stack.pop();
            eval_Stack.push(b1 and b2);
        }
        else if(postfix[i] == '^')
        {
            bool b1 = eval_Stack.top();
            eval_Stack.pop();
            bool b2 = eval_Stack.top();
            eval_Stack.pop();
            eval_Stack.push(b1 xor b2);
        }
        else if(postfix[i] == '!')
        {
            bool b1 = eval_Stack.top();
            eval_Stack.pop();
            eval_Stack.push(not b1);
        }
    }
    
    result = eval_Stack.top();
    eval_Stack.pop();

    return(0);
}


bool legal(char curr, char prev)
{
    if(curr == 'T')
    {
        switch(prev)
        {
            case 'T':
            case 'F':
            case ')': return false;
            default: return true;
        }
    }
    else if(curr == 'F')
    {
        switch(prev)
        {
            case 'F':
            case 'T':
            case ')': return false;
            default: return true;
        }
    }
    else if(curr == '&')
    {
        switch(prev)
        {
            case ')':
            case 'T':
            case 'F': return true;
            default: return false;
        }
    }
    else if(curr == '^')
    {
        switch(prev)
        {
            case ')':
            case 'T':
            case 'F': return true;
            default: return false;
        }
    }
    else if(curr == '!')
    {
        switch(prev)
        {
            case '!':
            case '(':
            case '&':
            case '^': return true;
            default: return false;
        }
    }
    else if(curr == '(')
    {
        switch(prev)
        {
            case '(':
            case '!':
            case '&':
            case '^': return true;
            default: return false;
        }
    }
    else if(curr == ')')
    {
        switch(prev)
        {
            case ')':
            case 'T':
            case 'F': return true;
            default: return false;
        }
    }
    
    return false;
}

bool validinfix(string test)
{
    if((test[0] == 'T' || test[0] == 'F' || test[0] == '!' || test[0] == '(') && (test[test.length()-1] == 'T' || test[test.length()-1] == 'F' || test[test.length()-1] == ')'))
    {
        for(int i=1; i<test.length(); i++)
        {
            bool b = legal(test[i], test[i-1]);
            if(!b)
                return false;
        }
        
        int parencounter = 0;
        
        for(int i=0; i<test.length(); i++)
        {
            if(test[i] == '(')
                parencounter++;
            else if(test[i] == ')')
                parencounter--;
        }
        
        if(parencounter == 0)
            return true;
    }
    return false;
}

int precedence(char op)
{
    if(op == '!')
        return 2;
    else if(op == '&')
        return 1;
    else if(op == '^')
        return 0;
    return -1;
}

string translate(string newinfix)
{
    string postfix = "";
    stack<char> operator_Stack;
    
    for(int i=0; i<newinfix.length(); i++)
    {
        switch(newinfix[i])
        {
            case 'T':
            case 'F':
                postfix+= newinfix[i];
                break;
            case '&':
            case '^':
                if(operator_Stack.empty())
                    operator_Stack.push(newinfix[i]);
                else
                {
                    while(!operator_Stack.empty() && (operator_Stack.top()!= '(') && (precedence(operator_Stack.top())>=precedence(newinfix[i])))
                    {
                        char top = operator_Stack.top();
                        operator_Stack.pop();
                        postfix+= top;
                    }
                    operator_Stack.push(newinfix[i]);
                }
                break;
            case '!':
            case '(':
                operator_Stack.push(newinfix[i]);
                break;
            case ')':
                while(operator_Stack.top()!= '(')
                {
                    char top = operator_Stack.top();
                    operator_Stack.pop();
                    postfix+= top;
                }
                operator_Stack.pop();
                break;
                
        }

    }
    
    while(!operator_Stack.empty())
    {
        char top = operator_Stack.top();
        operator_Stack.pop();
        postfix+= top;
    }

    return postfix;
}

int main()
{
    string pf;
    bool answer;

    assert(evaluate("T", pf, answer) == 0 && pf == "T" && answer);
    assert(evaluate("F", pf, answer) == 0 && pf == "F" && !answer);
    
    assert(evaluate("!T", pf, answer) == 0 && pf == "T!" && !answer);
    assert(evaluate("!F", pf, answer) == 0 && pf == "F!" && answer);
    
    assert(evaluate("T&T", pf, answer) == 0 && pf == "TT&" && answer);
    assert(evaluate("T&  F", pf, answer) == 0 && pf == "TF&" && !answer);
    assert(evaluate("F&T", pf, answer) == 0 && pf == "FT&" && !answer);
    assert(evaluate("F&F", pf, answer) == 0 && pf == "FF&" && !answer);
    
    assert(evaluate("T^T", pf, answer) == 0 && pf == "TT^" && !answer);
    assert(evaluate("T^F", pf, answer) == 0 && pf == "TF^" && answer);
    assert(evaluate("F^T", pf, answer) == 0 && pf == "FT^" && answer);
    assert(evaluate("F^F", pf, answer) == 0 && pf == "FF^" && !answer);
    
    assert(evaluate("F&F^T", pf, answer) == 0 && pf == "FF&T^" && answer);
    assert(evaluate("(F&F)^T", pf, answer) == 0 && pf == "FF&T^" && answer);
    assert(evaluate("T^(F&F)", pf, answer) == 0 && pf == "TFF&^" && answer);
    
    assert(evaluate("!F&T^T", pf, answer) == 0 && pf == "F!T&T^" && !answer);
    assert(evaluate("(!!!F&T)^(F&F^!!T)", pf, answer) == 0 && pf == "F!!!T&FF&T!!^^" && !answer);
    assert(evaluate("!F^F&(!!F&(!!F^F))", pf, answer) == 0 && pf == "F!FF!!F!!F^&&^" && answer);
    
    assert(evaluate("!!F&(T&T)^F", pf, answer) == 0 && pf == "F!!TT&&F^" && !answer);
    assert(evaluate("!F&!T^(!!F^!!T)", pf, answer) == 0 && pf == "F!T!&F!!T!!^^" && answer);
    assert(evaluate("T&T^F&F&F&T", pf, answer) == 0 && pf == "TT&FF&F&T&^" && answer);
    assert(evaluate("! ! !T&(T^T^(!F&T))", pf, answer) == 0 && pf == "T!!!TT^F!T&^&" && !answer);
    
    assert(evaluate("((T))", pf, answer) == 0 &&  pf == "T"  &&  answer);
    assert(evaluate("(T&(F^F)", pf, answer) == 1);
    assert(evaluate("T|F", pf, answer) == 1);
    assert(evaluate("", pf, answer) == 1);
    assert(evaluate("F  ^  !F & (T&F) ", pf, answer) == 0
           &&  pf == "FF!TF&&^"  &&  !answer);
    assert(evaluate(" F  ", pf, answer) == 0 &&  pf == "F"  &&  !answer);

    assert(evaluate("!F&(F^F^F)", pf, answer) == 0 && pf == "F!FF^F^&" && !answer);
    assert(evaluate("(!!F&T)^(!!T^F)", pf, answer) == 0 && pf == "F!!T&T!!F^^" && answer);
    assert(evaluate("!F&(!F^T^((F)))", pf, answer) == 0 && pf == "F!F!T^F^&" && !answer);
    
    assert(evaluate("T^", pf, answer) == 1);
    assert(evaluate("F F", pf, answer) == 1);
    assert(evaluate("TF", pf, answer) == 1);
    assert(evaluate("()", pf, answer) == 1);
    assert(evaluate("()T", pf, answer) == 1);
    assert(evaluate("T(F^T)", pf, answer) == 1);
    assert(evaluate("T(&T)", pf, answer) == 1);
    assert(evaluate("G^T", pf, answer) == 1);
    assert(evaluate("F^T)", pf, answer) == 1);
    assert(evaluate("^T)T(T^", pf, answer) == 1);
    assert(evaluate("TT^TT", pf, answer) == 1);
    assert(evaluate("F F T ^& T", pf, answer) == 1);
    assert(evaluate("FF&", pf, answer) == 1);
    assert(evaluate(")F^F(", pf, answer) == 1);
    assert(evaluate("", pf, answer) == 1);
    assert(evaluate("(T&)", pf, answer) == 1);
    assert(evaluate("(((())))", pf, answer) == 1);
    assert(evaluate("T^F!F)", pf, answer) == 1);
    
    cout << "Passed all tests" << endl;
}
