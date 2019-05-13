#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b1("tree/zeta");
    Bunch b2("tree");
    Bunch b3 = b2 - b1;

    EXPECT(b3.size() == 4)

    return 0;
}
