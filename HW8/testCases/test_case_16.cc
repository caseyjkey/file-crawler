#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("simple");

    EXPECT(b.entry(0)->path() == "simple")

    return 0;
}
