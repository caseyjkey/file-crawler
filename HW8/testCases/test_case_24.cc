#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("tree");

    multiset<string> entires;
    for (auto fp : b) {
        entires.insert(string(fp->mtime()));
    }

    string mtime_lines;
    mtime_lines += entires;
 
    CHECK_LINES(mtime_lines, "2019-01-06T12:09:47\n"
                             "2019-01-06T12:11:10\n"
                             "2019-01-06T12:11:36\n"
                             "2019-01-06T12:12:12\n"
                             "2019-01-06T12:16:48\n"
                             "2019-05-05T12:33:59\n"
                             "2019-05-05T12:39:53\n"
                             "2019-05-05T12:40:29\n"
                             "2019-05-05T12:49:49\n")

    return 0;
}
