#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    const Bunch b("empty");

    if (b.empty()) {
        info = "const b should be empty";
        return 1;
    }

    return 0;
}