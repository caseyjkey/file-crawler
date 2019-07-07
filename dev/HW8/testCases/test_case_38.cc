#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    const Bunch b1("tree");
    Bunch b2 = b1;

    EXPECT(b1 == b2)

    return 0;
}
