#include <iostream>
#include <cstdlib>

using namespace std;

class Foo {
  public:
    static void *operator new(size_t size) {
	void *p = new char[size];
	if (min==nullptr || p < min)
	    min = p;
	if (max==nullptr || p > max)
	    max = p;
	cout << "new(" << size << ") returns " << p << '\n';
	return p;
    }

    static void operator delete(void *p, size_t size) {
	cout << "delete(" << p << ", " << size << ")\n";
	if (p == nullptr)		// It's ok to delete a null pointer.
	    return;

	if (min == nullptr) {
	    cerr << "ERROR: attempt to delete a Foo, "
		 << "but no Foo was ever allocated.\n";
	    exit(1);
	}
	else if (p < min || p > max) {
	    cerr << "ERROR: attempt to delete a Foo, "
		 << "but " << p << " is out of range.\n";
	    exit(1);
	}
	else if (long(p) & 07) { // checking if the pointer is even -> valid
	    // Verify our rash assumption:
	    static_assert(sizeof(long) == sizeof(p),
			  "assume long and void * are the same size");
	    cerr << "ERROR: attempt to delete a Foo, "
		 << "but " << p << " has bad alignment.\n";
	    exit(1);
	}

	delete[] static_cast<char *>(p);
    }

    static void *min, *max;
    char zot[4000];
};

void *Foo::min, *Foo::max;

int main() {
    double *p = new double;
    Foo *q = new Foo;
    int z;
    Foo *r = (Foo *) &z;
    delete p;		// fine
    delete q;		// fine
    delete r;		// bad: this is a stack address
    return 0;
}
