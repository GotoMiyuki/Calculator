#include<iostream>
#include<string>
#include"polynomial.cpp"
#include"expression.cpp"
using namespace std;

void polynomialMenu(){
    Polynomial p1,p2,result;
    int choice;
    double x;

    while(1){
        cout<<"Sparse Polynomial Calculator"<<endl;
        cout<<"=============================="<<endl;
        cout<<"1. input Polynomial 1"<<endl;
        cout<<"2. input Polynomial 2"<<endl;
        cout<<"3. display Polynomial"<<endl;
        cout<<"4. output P1 + P2"<<endl;
        cout<<"5. output P1 - P2"<<endl;
        cout<<"6. output P1 * P2"<<endl;
        cout<<"7. output Derivative"<<endl;
        cout<<"8. calculate P(x)"<<endl;
        cout<<"0. return Menu"<<endl;
        cout<<"=============================="<<endl;
        cout<<"enter your choice"<<endl;
        cin>>choice;

        if (choice == 0) break;
        switch (choice) {
            case 1:
                cout<<"Get Polynomial 1"<<endl;
                p1.clear();
                p1.input();
                break;
            case 2:
                cout<<"Get Polynomial 2"<<endl;;
                p2.clear();
                p2.input();
                break;
            case 3:
                cout<<"Which polynomial do you want to display ?"<<endl;
                int choice1;
                cin>>choice1;
                if(choice1 == 1){
                    cout<<"P1(x) = ";
                    p1.display();
                }else if(choice1 == 2){
                    cout<<"P2(x) = ";
                    p2.display();
                }break;
            case 4:
                result.clear();
                result = p1 + p2;
                cout<<"P1 + P2 = ";
                result.display();
                break;
            case 5:
                result.clear();
                result = p1 - p2;
                cout<<"P1 - P2 = ";
                result.display();
                break;
            case 6:
                result.clear();
                result = p1 * p2;
                cout<<"P1 * P2 = ";
                result.display();
                break;
            case 7:
                result.clear();
                cout<<"Which polynomial do you want to derivative ?"<<endl;
                int choice2;
                cin>>choice2;
                if(choice2 == 1){
                    cout<<"P1'(x) = ";
                    result = p1.Derivative();
                }else if(choice2 ==2){
                    cout<<"P2'(x) = ";
                    result = p2.Derivative();
                }result.display();
                break;
            case 8:
                result.clear();
                cout<<"Which polynomial do you want to deal ?"<<endl;
                int choice3;
                cin>>choice3;
                cout<<"input x : ";
                cin>>x;
                if(choice3 == 1){
                    cout<<"P1(" << x << ") = "<<p1.evaluate(x)<<endl;
                }else if(choice3 ==2){
                    cout<<"P2(" << x << ") = "<<p2.evaluate(x)<<endl;
                }
                break;
            default:
                cout<<"invalid input"<<endl;
        }
    }
}

void expressionMenu(){
    string input;
    Expression expr;
    cin.ignore();
    cout<<"Expression Calculator"<<endl;
    cout<<"=============================="<<endl;
    cout<<"input an expression(support + - * / ^ and parenthese): "<<endl;
    cout<<"input # to stop entering"<<endl;
    char ch;
    while(cin.get(ch)){
        if(ch == '#') break;
        input += ch;
    }
    expr.setInfix(input);

    try{
        double result = expr.evaluate();
        cout<<"outcome: "<<result<<endl;
    } catch (const exception& e) {
        cerr<<e.what()<<endl;
    }
}

int main(){
    int choice;
    while(1){
        cout<<"      Main Menu of Multi-Calculator"<<endl;
        cout<<"1. Sparse Polynomial Calculator"<<endl;
        cout<<"2. Expression Calculator"<<endl;
        cout<<"enter 0 to quit"<<endl;
        cout<<"=============================="<<endl;
        cout<<"enter your choice"<<endl;
        cin>>choice;

        switch(choice){
            case 1: polynomialMenu(); break;
            case 2: expressionMenu(); break;
            case 0: cout<<"Quited"<<endl; return 0;
            default: cout<<"Invalid input"<<endl;
        }
    }
}