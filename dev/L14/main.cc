#include "hset.h"
#include <cstdlib>			// for srand() and rand()
#include <iostream>
#include <time.h>			// for time() (duh)

using namespace std;

int main() {
    srand(time(nullptr));		// Seed the random number generator

    hset<int, 10> h;
    cout << boolalpha << h.empty() << "\n";
    for (int j=0; j<50; j++)		// Put some random numbers into the set
	h.insert(rand() % 100);		// Each number is 0–99
    h.dump();				// Let’s see what’s in there

    cout << "size=" << h.size() << ':';
    for (auto v : h)
	cout << ' ' << v;
    cout << '\n';

    hset<string, 10> hs;
    for (auto s : {"alpha", "beta", "gamma", "delta", "epsilon", "zeta", "eta"})
	hs.insert(s);
    hs.dump();

    cout << "size=" << hs.size();
    for (auto v : hs)
	cout << ' ' << v;
    cout << '\n';

    hset<double, 10> hd;
    for( auto d : {1.0, 2.2, 3.0, 5.6, 7.2, 1.4} ){
        hd.insert(d);
    }
    hd.dump();
}
