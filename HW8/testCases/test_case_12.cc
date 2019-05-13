#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("simple");

    auto it = b.begin();
    EXPECT(it++ == b.begin())

    return 0;
}
