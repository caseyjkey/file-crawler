#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    const Bunch b("simple");

    EXPECT(b.entry(0) == *b.begin())

    return 0;
}
