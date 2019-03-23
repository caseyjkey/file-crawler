#include "Bunch.h"
#include <pwd.h>
#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;

int main() {
    const auto home = getpwnam("cs253")->pw_dir;    // cs253’s home dir
    try {
	if (chdir(home) != 0)			    // go to ~cs253
	    throw "Can’t chdir to "s + home;
	Bunch b("pub/tree");			    // relative path from here
	b.format("%p\t%s\t%M\t%n");		    // align output with tabs
	for (size_t i=0; i<b.size(); i++)
	    cout << b.entry(i) << '\n';
    }
    catch (string err) {
	cerr << "ERROR: " << err << '\n';
    }
}
