#include <iostream>

using namespace std;

class Foo {
public:
    static void *operator new(size_t size) {
	void *p = new char[size];
	cout << "new(" << size << ") returns " << p << '\n';
	return p;
    }

    static void operator delete(void *p, size_t size) {
	cout << "delete(" << p << ", " << size << ")\n";
	delete[] static_cast<char *>(p);
    }

    char zot[4000];
};

int main() {
    double *p = new double;
    Foo *q = new Foo;
    Foo *r = new Foo;
    delete p;
    delete q;
    delete r;
    return 0;
}
