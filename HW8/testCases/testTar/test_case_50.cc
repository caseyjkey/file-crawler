#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("tree2");

    try {
        auto fp = b.entry(20);
        b += Bunch(fp->path());
    } EXPECT_THROWN_STRING_TO_CONTAIN("20")

    return 0;
}
