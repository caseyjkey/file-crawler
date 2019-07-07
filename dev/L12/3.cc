#include <iostream>
#include <limits>
#include <map>
#include <iomanip>
#include <string>

using namespace std;

template <typename T>
class BarGraph {
  public:
    void operator+=(const T &datum) {
        data[datum]++;
    }
    void dump() const {
	for (const auto &val : data)
	    cout << right << setw(10) << val.first << ' '
		 << string(val.second, '*') << '\n';
        cout << '\n';
    }

    template <typename U>
    friend ostream& operator<<(ostream &stream, const BarGraph<U> &rhs) {
        for(const auto &val : rhs.data)
             stream << right << setw(10) << val.first << ' '
	     << string(val.second, '*') << '\n';
        return stream;
    }

  private:
    map<T, unsigned> data;
};

template <>
class BarGraph<bool> {
    unsigned false_count = 0, true_count = 0;
  public:
    void operator+=(bool datum) {
	datum ? true_count++ : false_count++;
    }
    void dump() const {
        cout << "     false " << string(false_count, '*') << "\n"
                "      true " << string(true_count,  '*') << "\n\n";
    }
};

template <>
class BarGraph<char> {
  unsigned char data[numeric_limits<char>::max()] = {0};  
  public:
    void operator+=(const char &datum) {
	data[(unsigned char)(datum)]++;
    }
    
    friend ostream& operator<<(ostream &stream, const BarGraph<char> &rhs) {
        for(const auto &val : rhs.data)
             if(val != 0) stream << char (val) << setw(10)
	     << string(val, '*') << '\n';
        return stream;
    }
};

int main() {
    BarGraph<int> alpha;
    alpha += 12;
    alpha += 6;
    alpha += 4;
    alpha += 6;
    alpha.dump();

    BarGraph<double> beta;
    beta += 3.14;
    beta += 2.71828;
    beta += 6.023e23;
    beta += 2.71828;
    beta.dump();

    BarGraph<bool> gamma;
    gamma += false;
    gamma += true;
    gamma += false;
    gamma += true;
    gamma += true;
    gamma.dump();

    BarGraph<char> delta;
    delta += 'G';
    delta += 'e';
    delta += 'o';
    delta += 'f';
    delta += 'f';
    cout << delta;

    return 0;
}
