#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b1("tree");
    Bunch b2("tree2");

    b1 += Bunch("simple");
    b2 += Bunch("simple");

    EXPECT((b1 + b2) - (b1 - b2) - (b2 - b1) == Bunch("simple"))

    return 0;
}
