#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("simple");

    try {
        Fing s = b.entry(500);
    } EXPECT_THROWN_STRING_TO_CONTAIN("500")

    return 0;
}
