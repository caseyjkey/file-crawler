// This program illustrates the use of the fix container.
// It also demonstrates the range-based for loop.

#include "fix.h"
#include <iostream>

using namespace std;

int main() {
    fix<double, 4> f;
    f[0] = 1.2; f[1] = 3.4; f[2] = 5.6;

    // This is how a C programmer would display the contents of f:
    for (unsigned i=0; i<f.size(); i++)
	cout << f[i] << ' ';
    cout << '\n';

    // This is how a C++2003 programmer would display the contents of f:
    for (fix<double,4>::iterator it=f.begin(); it!=f.end(); ++it)
	cout << *it << ' ';
    cout << '\n';

    // This is how a C++2011 programmer would display the contents of f:
    // This actually uses an iterator and calls begin() and end(), just like
    // the previous code, but it’s much more readable.
    for (auto v : f)
	cout << v << ' ';
    cout << '\n';
    
    // If the container contains large objects, and speed matters,
    // then traverse using a const reference:
    for (const auto &v : f)
	cout << v << ' ';
    cout << '\n';
}
