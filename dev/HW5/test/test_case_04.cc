#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    const Bunch b("empty");

    if (b.size() != 1) {
        info = "const bunch should have size 1.";
        return 1;
    }

    return 0;
}
