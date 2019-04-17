#include "Bunch.h"	// Do this first for best testing.
#include <sys/types.h>	// for getpwnam
#include <pwd.h>	// for getpwnam
#include <unistd.h>	// for chdir
#include <cassert>	// assert
#include <iostream>	// for cout, cerr
#include <iomanip>	// for setw
#include <string>	// What a mystery!

using namespace std;

void dump(string label, const Bunch &b) {
    cout << label << ":\n";
    for (size_t i=0; i<b.size(); i++) {
	const Fing *p = b.entry(i);
	cout << string(p->perms()) << ' ' << p->path() << '\n';
    }
    cout << '\n';
}

int main() {
    try {
	const auto home = getpwnam("cs253")->pw_dir; // cs253’s home dir
	if (chdir(home) != 0)			     // go to ~cs253
	    throw "Can’t chdir to "s + home;
	const Bunch b1("pub/tree/alpha/iota");
	Bunch b2("pub/tree2/tau");
	dump("b1", b1);
	b2 += Bunch("/etc/group");
	dump("b2", b2);
	dump("b1+b2", b1+b2);
	Bunch b3(b2);
	b3 += b1;
	b3 += b1;				// Yes, I did it twice.
	b3 -= Bunch("/etc/resolv.conf");	// Should have no effect
	assert(b1.size() == 3);
	assert(b2.size() == 3);
	assert((b1+b2).size() == 6);
	assert(b1+b2 == b3);
	assert(b1 == b3-b2);
    cout << "preceding yeet" << endl;
    cout << "----------- b1 before  -----------\n";
    cout << b1 << endl;
	assert(b1);
    cout << "----------- b1 after -------------\n";
    cout << b1 << endl;
	assert(b2);
	assert(b3);
	assert(!(b1-b1));
    }
    catch (string err) {
	cerr << "ERROR: " << err << '\n';
    }
}
