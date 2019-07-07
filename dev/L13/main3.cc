#include "fix3.h"		// Use fix3.h, not fix.h
#include <iostream>
using std::cout;

int main() {
    fix<int, 3> f;
    f[0] = 11;
    f[1] = 22;
    f[2] = 33;
    fix<int,3>::iterator it = f.begin();
    ++it;	// Now it “points” to f[1]
    ++it;	// Now it “points” to f[2]
    ++it;	// Now it “points” to f[3], the same as f.end(), which is ok.
    ++it;	// Now it “points” past f.end(); that’s bad.
    cout << "f[4]: " << *it << '\n';	// This is very wrong.
}
