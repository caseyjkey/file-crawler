#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    const Bunch b("simple");

    EXPECT(b.size() == 1)

    return 0;
}
