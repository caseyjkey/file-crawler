#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("simple");

    size_t sz { b.size() };

    EXPECT(sz == 1)

    return 0;
}
