#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    const Bunch b1("tree");
    const Bunch b2("tree2");
    const Bunch b3 = b1 + b2;

    EXPECT(b1.size() + b2.size() == b3.size())
    return 0;
}
