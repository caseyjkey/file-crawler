#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("simple");

    if (b.entry(0).size() != 19) {
        info = "fing b should have size 19";
        return 1;
    }

    return 0;
}