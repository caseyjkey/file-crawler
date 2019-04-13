#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b;

    if (true) {
        info = "This should not compile. Bunch has no default constructor.";
        return 1;
    }

    return 0;
}