#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b1("tree");
    Bunch b2("tree2");

    b2 = b1;

    b1 -= b2;

    EXPECT(b1.empty())

    return 0;
}
