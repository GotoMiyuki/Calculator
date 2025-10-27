#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

static const double EPS = 1e-9;

struct Term {
    double coef;
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

        static string formatNumber(double v) {
            // format with fixed precision then trim trailing zeros
            ostringstream oss;
            oss << fixed << setprecision(6) << v;
            string s = oss.str();
            if (s.find('.') != string::npos) {
                while (!s.empty() && s.back() == '0') s.pop_back();
                if (!s.empty() && s.back() == '.') s.pop_back();
            }
            if (s == "-0") s = "0";
            return s;
        }

    public:
        Polynomial(): head(nullptr){}

        void clear(){
            Term* current = head;
            while(current){
                Term* temp = current;
                current = current->next;
                delete temp;
            } head = nullptr;
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

        // fixed move ctor/assign
        Polynomial(Polynomial&& other) {
            head = other.head;
            other.head = nullptr;
        }

        Polynomial& operator=(Polynomial&& other) {
            if (this == &other) return *this;
            this->clear();
            head = other.head;
            other.head = nullptr;
            return *this;
        }

        void insertTerm(double coef, int exp) {
            if (fabs(coef) < EPS) return;

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
                if(fabs(current->coef) < EPS){
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
                double coef;
                int exp;
                cout<<"exponent: ";
                if (!(cin >> expInput)) return;
                if(expInput == "#") return;
                try{
                    exp = stoi(expInput);
                }catch(...){
                    cout<<"Invalid input. Please enter an integer exponent or '#' to stop."<<endl;
                    continue;
                }
                cout<<"coefficient of x^"<<exp<<": ";
                if (!(cin >> coef)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid coefficient. Try again." << endl;
                    continue;
                }
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
                double c = current->coef;
                int e = current->exp;

                if(!first){
                    cout << (c >= 0 ? "+" : "-");
                } else {
                    if (c < 0) cout << "-";
                }

                double absC = fabs(c);

                if(e == 0){
                    cout << formatNumber(absC);
                } else if (fabs(absC - 1.0) < EPS) {
                    // coefficient 1 or -1: omit number
                    if (e == 1) cout << "x";
                    else cout << "x^" << e;
                } else {
                    if (e == 1) cout << formatNumber(absC) << "x";
                    else cout << formatNumber(absC) << "x^" << e;
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

        Polynomial operator+(const Polynomial& other) const {
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
                    double sum = p1->coef + p2->coef;
                    if(fabs(sum) >= EPS) result.insertTerm(sum, p1->exp);
                    p1 = p1->next;
                    p2 = p2->next;
                }
            }
            return result;
        }
        
        Polynomial operator-(const Polynomial& other) const {
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
                    double diff = p1->coef - p2->coef;
                    if(fabs(diff) >= EPS) result.insertTerm(diff, p1->exp);
                    p1 = p1->next;
                    p2 = p2->next;
                }
            }
            return result;
        }

        Polynomial operator*(const Polynomial& other) const {
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

        Polynomial Derivative() const {
            Polynomial result;
            Term* p1 = this->head;
            while(p1){
                if(p1->exp != 0){
                    result.insertTerm(p1->coef * p1->exp, p1->exp - 1);
                }
                p1 = p1->next;
            }
            return result;
        }

        double evaluate(double x) const {
            Term* p1 = this->head;
            double sum = 0.0;
            while(p1){
                sum += pow(x, p1->exp) * p1->coef;
                p1 = p1->next;
            }
            return sum;
        }

        string toHTML() const{
            if(!head){
                return "0";
            }

            string html;
            Term* current = head;
            bool first = true;
            
            while(current){
                double c = current->coef;
                int e = current->exp;
                if(!first && c > 0) html += "+";
                if(c < 0) html += "-";

                double absC = fabs(c);

                if(e == 0){
                    html += formatNumber(absC);
                } else if (fabs(absC - 1.0) < EPS) {
                    if (e == 1) html += "x";
                    else html += "x<sup>" + to_string(e) + "</sup>";
                } else if(e == 1){
                    html += formatNumber(absC) + "x";
                } else {
                    html += formatNumber(absC) + "x<sup>" + to_string(e) + "</sup>";
                }

                first = false;
                current = current->next;
            }
            return html;
        }

        // optional helper to check emptiness
        bool isEmpty() const { return head == nullptr; }
};
