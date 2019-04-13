#include <iostream>
#include <sstream>

using namespace std;

class Number {
  public:
    Number() = default;
    Number(const Number &) = default;
    Number(int v) : value(v) {}
    Number &operator=(int n) { value = n; return *this; }
    operator int() const { return value; }
  private:
    int value = 0;			// default value unless otherwise given
};

istream &operator>>(istream &is, Number &rhs) {
    int n;
    is >> n;
   
    rhs = n;
    if(is.good())
        return is;
    if(!is.good())
        is.clear();

    string word;
    is >> word;
    if(word == "three")
         rhs = 3;
    if(word == "five")
         rhs = 5;

    return is;
}


int main() {
    Number n = 666;
    istringstream ss("12 34 three 789 five");

    while (ss >> n)
	cout << n << ' ';
    cout << '\n';
}
