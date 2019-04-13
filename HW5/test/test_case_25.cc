#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b(".hidden_dir");
    b.all();

    multiset<string> student_entries;
    for (size_t i=0; i<b.size(); i++) {
        student_entries.insert(b.entry(i).path());
    }

    string student_lines;
    student_lines += student_entries;

    CHECK_LINES(student_lines, ".hidden_dir\n"
                               ".hidden_dir/.hidden\n")

    return 0;
}
