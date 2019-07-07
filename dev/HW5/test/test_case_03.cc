#include "Bunch.h"
#include "test.h"

using namespace std;
// Fing copy constructor.
int run_test_case(string &info) {
    Bunch b("empty");

    size_t sz {b.size()};

    if (sz != 1) {
        info = "b size should be 1.";
        return 1;
    }

    return 0;
}