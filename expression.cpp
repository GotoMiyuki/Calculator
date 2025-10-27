#include <iostream>
#include <cmath>
#include <string>
#include <stdexcept>
using namespace std;

template<typename T>
struct Stack {
    T data[100];
    int top = -1;

    bool empty() const{return top == -1;}

    void push(T x){
        if(top >= 99) throw runtime_error("Error: Stack overflow");
        data[++top] = x;
    }

    void pop(){
        if(empty()) throw runtime_error("Error: Stack underflow");
        top--;
    }

    T peek()const {
        if(empty()) throw runtime_error("Error: Stack is empty");
        return data[top];
    }

    int size()const {return top + 1;}
};

struct Array {
    string data[200];
    int length = 0;

    void push_back(const string& s) {
        if(length >= 200) throw runtime_error("Error: Array overflow");
        data[length++] = s;
    }
};

bool priority(char p, char q) {
    if(p == '(') return false;
    if(p == '^' && q == '^') return false;
    if(p == '^') return true;
    if(p == '*' || p == '/') return true;
    if((p == '+' || p == '-') && (q == '+' || q == '-')) return true;
    return false;
}

class Expression {
private:
    string infix;

public:
    Expression() = default;
    Expression(string str): infix(str) {}

    void setInfix(string str) { infix = str; }
    string getInfix() const { return infix; }

    Array infixToPostfix() {
        Array outcome;
        Stack<char> S;
        int n = infix.length();

        for(int i = 0; i < n; i++) {
            if(isspace(infix[i])) continue;

            if(isdigit(infix[i])){
                string num;
                while(i < n && isdigit(infix[i])){
                    num += infix[i];
                    i++;
                }
                i--;
                outcome.push_back(num);
            }
            else if(infix[i] == '(') {
                S.push('(');
            }
            else if(infix[i] == ')') {
                while(!S.empty() && S.peek() != '(') {
                    outcome.push_back(string(1, S.peek()));
                    S.pop();
                }
                if(!S.empty()) S.pop();
                else throw runtime_error("Error: Unmatched parenthesis ')'");
            }
            else if(infix[i] == '+' || infix[i] == '-' ||
                     infix[i] == '*' || infix[i] == '/' || infix[i] == '^') {
                while(!S.empty() && S.peek() != '(' && priority(S.peek(), infix[i])){
                    outcome.push_back(string(1, S.peek()));
                    S.pop();
                }
                S.push(infix[i]);
            }
            else{
                throw runtime_error(string("Error: Invalid character '") + infix[i] + "'");
            }
        }

        while(!S.empty()){
            if(S.peek() != '(')
                outcome.push_back(string(1, S.peek()));
            S.pop();
        }
        return outcome;
    }

    double calculatePostfix(const Array& postfix) {
        Stack<double> S;
        for (int i = 0; i < postfix.length; ++i) {
            const string& token = postfix.data[i];
            if(isdigit(token[0])) {
                S.push(stod(token));
            }else{
                if(S.size() < 2)
                    throw runtime_error("Error: Insufficient operands");
                double b = S.peek(); S.pop();
                double a = S.peek(); S.pop();
                double result;
                if(token == "+") result = a + b;
                else if(token == "-") result = a - b;
                else if(token == "*") result = a * b;
                else if(token == "/") {
                    if(b == 0) throw runtime_error("Error: Division by zero");
                    result = a / b;
                }
                else if(token == "^") result = pow(a, b);
                else throw runtime_error("Error: Unknown operator");
                S.push(result);
            }
        }
        if(S.size() != 1) throw runtime_error("Error: Invalid expression");
        return S.peek();
    }

    double evaluate(){
        Array postfix = infixToPostfix();
        return calculatePostfix(postfix);
    }
};