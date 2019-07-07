#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("tree2/alpha");

    const Fing * fp = b.entry(0);
    EXPECT(dynamic_cast<const Regular *>(fp) != nullptr)

    return 0;
}
