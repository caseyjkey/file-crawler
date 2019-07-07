#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b1("tree");
    Bunch b2("tree");

    EXPECT(b1 == b2)

    return 0;
}
