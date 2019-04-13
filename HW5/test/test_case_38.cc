#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("tree");
    // b.format("%n");

    multiset<string> student_entries;
    for (size_t i=b.size() - 1; ; i--) {
        student_entries.insert(b.entry(i).path());
        if (i == 0) break;
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
