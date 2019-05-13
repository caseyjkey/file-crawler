#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    const Bunch b("tree2");

    multiset<string> entries;
    for (auto fp : b) {
        entries.insert(fp->type());
    }

    string type_lines;
    type_lines += entries;

    CHECK_LINES(type_lines, "directory\n"
                            "directory\n"
                            "regular\n"
                            "regular\n"
                            "symlink\n")

    return 0;
}
