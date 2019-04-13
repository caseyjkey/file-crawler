#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    const Bunch b("simple");

    time_t atime = b.entry(0).ctime();
    if (atime < 1553821216){
    	info = "ctime can not be less than 1553821216.";
        return 1;
    }

    return 0;
}