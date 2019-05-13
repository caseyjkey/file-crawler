#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b1("tree/zeta");
    Bunch b2("tree");

    b1 += b2;

    EXPECT(b1.size() == 9)

    return 0;
}
