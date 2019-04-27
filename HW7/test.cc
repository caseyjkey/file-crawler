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
    for (const Fing *p : b)
	cout << string(p->perms()) << ' ' << p->path() << '\n';
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
	
    }
    catch (string err) {
	cerr << "ERROR: " << err << '\n';
    }
}
