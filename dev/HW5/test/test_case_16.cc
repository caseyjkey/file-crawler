#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    try {
        Bunch b("ghost");
    } EXPECT_THROWN_STRING_TO_CONTAIN("ghost")

    return 0;
}