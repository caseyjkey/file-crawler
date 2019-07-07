#include "Bunch.h"
#include "test.h"

using namespace std;
// Fing copy constructor.
int run_test_case(string &info) {
    Bunch b("simple");

    size_t sz {b.entry(0).size()};

    if (sz != 19) {
        info = "b size should be 19.";
        return 1;
    }

    return 0;
}