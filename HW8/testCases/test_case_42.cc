#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("tree2/alpha");

    const Fing * fp { *b.begin() };
    EXPECT(fp->type() == "regular"s)

    return 0;
}
