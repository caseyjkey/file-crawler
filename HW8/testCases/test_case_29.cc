#include "Bunch.h"
#include "test.h"
#include <iostream>

using namespace std;

void dump(string label, const Bunch &b) {
        cout << label << ":\n";
        for (auto p : b)
        cout << string(p->perms()) << ' ' << p->path() << endl;
        cout << '\n';
    }

int run_test_case(string &info) {
    Bunch b1("tree");
    Bunch b2("tree/zeta");

    dump("B2: ", b2);
    dump("B1: ", b1);
    cout << "Before: " << b1.size() << endl;
    b1 = b1 - b2;
    dump("B1: ", b1);
    cout << "After: " << b1.size() << endl;

    EXPECT(b1.size() == 4)

    return 0;
}
