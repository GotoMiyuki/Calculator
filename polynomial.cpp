#include<iostream>
#include<cmath>
using namespace std;

struct Term {
    int coef;
    int exp;
    Term* next;
};

class Polynomial {
    private:
        Term* head;

        Term* cloneList(const Term* src) {
            if (!src) return nullptr;
            Term* newHead = new Term;
            newHead->coef = src->coef;
            newHead->exp = src->exp;
            newHead->next = nullptr;

            Term* tail = newHead;
            const Term* cur = src->next;
            while (cur) {
                tail->next = new Term;
                tail->next->coef = cur->coef;
                tail->next->exp = cur->exp;
                tail->next->next = nullptr;
                tail = tail->next;
                cur = cur->next;
            }
            return newHead;
        }

    public:
        Polynomial(): head(nullptr){}

        void clear(){
            Term* current = head;
            while(current){
                Term* temp = current;
                current = current->next;
                delete temp;
            }head = nullptr;
        }

        Polynomial(const Polynomial& other) : head(nullptr) {
            head = cloneList(other.head);
        }

        Polynomial& operator=(const Polynomial& other) {
            if (this == &other) return *this;
            this->clear();
            head = cloneList(other.head);
            return *this;
        }

        Polynomial(Polynomial&& other){
            other.head = nullptr;
        }

        Polynomial& operator=(Polynomial&& other){
            if (this == &other) return *this;
            this->clear();
            head = other.head;
            other.head = nullptr;
            return *this;
        }

        void insertTerm(int coef, int exp) {
            if (coef == 0) return; 

            Term* newTerm = new Term;
            newTerm->coef = coef;
            newTerm->exp = exp;
            newTerm->next = nullptr;

            if(!head){
                head = newTerm;
                return;
            }

            if(exp > head->exp){
                newTerm->next = head;
                head = newTerm;
                return;
            }

            Term* prev = nullptr;
            Term* current = head;
            while(current && current->exp > exp){
                prev = current;
                current = current->next;
            }

            if(current && current->exp == exp){
                current->coef += coef;
                if(current->coef == 0){
                    if(prev) prev->next = current->next;
                    else head = current->next;
                    delete current;
                }
                delete newTerm;
                return;
            }

            newTerm->next = current;
            if(prev) prev->next = newTerm;
            else head = newTerm;
        }

        void input(){
            cout<<"Input terms for the polynomial (enter '#' to stop):"<<endl;
            while(1){
                string expInput;
                int coef, exp;
                cout<<"exponent: ";
                cin>>expInput;
                if(expInput == "#") return;
                try{
                    exp = stoi(expInput);
                }catch(invalid_argument&){
                    cout<<"Invalid input. Please enter an integer exponent or '#' to stop."<<endl;
                    continue;
                }
                cout<<"coefficient of x^"<<exp<<": ";
                cin>>coef;
                insertTerm(coef, exp);
            }
        }

        void display() const{
            if(!head){
                cout << "0" << endl;
                return;
            }

            Term* current = head;
            bool first = true;
            while(current){
                if(!first && current->coef > 0) cout << "+";
                if(current->coef < 0) cout << "-";
                
                if(current->exp == 0){
                    cout << abs(current->coef);
                }else if(abs(current->coef) == 1 && current->exp == 1){
                    cout << "x";
                }else if(abs(current->coef) == 1){
                    cout << "x^" << current->exp;
                }else if(current->exp == 1){
                    cout << abs(current->coef) << "x";
                }else{
                    cout << abs(current->coef) << "x^" << current->exp;
                }
                first = false;
                current = current->next;
            }
            cout << endl;
        }

        ~Polynomial() {
            Term* current = head;
            while(current){
                Term* temp = current;
                current = current->next;
                delete temp;
            }
        }

        Polynomial operator+(const Polynomial& other){
            Polynomial result;
            Term* p1 = this->head;
            Term* p2 = other.head;
            while(p1 || p2){
                if(p1 && (!p2 || p1->exp > p2->exp)){
                    result.insertTerm(p1->coef, p1->exp);
                    p1 = p1->next;
                }else if(p2 && (!p1 || p2->exp > p1->exp)){
                    result.insertTerm(p2->coef, p2->exp);
                    p2 = p2->next;
                }else if(p1 && p2){
                    if(p1->coef + p2->coef != 0) result.insertTerm(p1->coef + p2->coef, p1->exp);
                    p1 = p1->next;
                    p2 = p2->next;
                }
            }
            return result;
        }
        
        Polynomial operator-(const Polynomial& other){
            Polynomial result;
            Term* p1 = this->head;
            Term* p2 = other.head;
            while(p1 || p2){
                if(p1 && (!p2 || p1->exp > p2->exp)){
                    result.insertTerm(p1->coef, p1->exp);
                    p1 = p1->next;
                }else if(p2 && (!p1 || p2->exp > p1->exp)){
                    result.insertTerm(-p2->coef, p2->exp);
                    p2 = p2->next;
                }else if(p1 && p2){
                    if(p1->coef != p2->coef) result.insertTerm(p1->coef - p2->coef, p1->exp);
                    p1 = p1->next;
                    p2 = p2->next;
                }
            }
            return result;
        }

        Polynomial operator*(const Polynomial& other){
            Polynomial result;
            Term* p1 = this->head;
            while(p1){
                Term* p2 = other.head;
                while(p2){
                    result.insertTerm(p1->coef * p2->coef, p1->exp + p2->exp);
                    p2 = p2->next;
                }
                p1 = p1->next;
            }
            return result;
        }

        Polynomial Derivative() const{
            Polynomial result;
            Term* p1 = this->head;
            while(p1){
                if(p1->exp != 0){
                    result.insertTerm(p1->coef * p1->exp, p1->exp - 1);
                }p1 = p1->next;
            }return result;
        }

        double evaluate(double x) const{
            Term* p1 = this->head;
            double sum = 0.0;
            while(p1){
                sum += pow(x, p1->exp) * p1->coef;
                p1 = p1->next;
            }return sum;
        }

        string toHTML() const{
            if(!head){
                return "0";
            }

            string html;
            Term* current = head;
            bool first = true;
            
            while(current){
                if(!first && current->coef > 0) html += "+";
                if(current->coef < 0) html += "-";
                if(current->exp == 0){
                    html += to_string(abs(current->coef));
                }else if(current->coef == 1 && current->exp == 1){
                    html += "x";
                }else if(current->coef == 1){
                    html += "x<sup>" + to_string(current->exp) + "</sup>";
                }else if(current->exp == 1){
                    html += to_string(abs(current->coef)) + "x";
                }else{
                    html += to_string(abs(current->coef)) + "x<sup>" + to_string(current->exp) + "</sup>";
                }
                first = false;
                current = current->next;
            }
            return html;
        }
};