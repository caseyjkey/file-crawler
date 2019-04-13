#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    const Bunch b1("tree");

    const Bunch b2 = b1;

    multiset<string> student_entries;
    for (size_t i=0; i<b2.size(); i++) {
        student_entries.insert(b2.entry(i).path());
    }

    string student_lines;
    student_lines += student_entries;

    CHECK_LINES(student_lines, "tree\n"
                               "tree/alpha\n"
                               "tree/alpha/epsilon\n"
                               "tree/alpha/iota\n"
                               "tree/alpha/iota/kappa\n"
                               "tree/alpha/iota/omega\n"
                               "tree/beta\n"
                               "tree/delta\n")

    return 0;
}
