#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    const Bunch b("simple");

    time_t atime = b.entry(0).atime();
    if (atime < 1554567600){
    	info = "atime can not be less than 1554567600.";
        return 1;
    }

    return 0;
}