#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("simple");

    if (b.empty()) {
        info = "b should not be empty";
        return 1;
    }

    return 0;
}
