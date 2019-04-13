#include "Bunch.h"
#include "test.h"

using namespace std;
// Fing copy constructor.
int run_test_case(string &info) {
    Bunch b("empty");
    Fing fg = b.entry(0);

    if (b.entry(0).path() != fg.path()) {
        info = "Error with Fing copy constructor.";
        return 1;
    }

    return 0;
}