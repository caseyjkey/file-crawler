#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    const string path = "with_hidden_dir";
    Bunch b(path);
    // b.all(false);

    multiset<string> student_entries;
    for (size_t i=0; i<b.size(); i++) {
        student_entries.insert(b.entry(i).path());
    }

    string student_lines;
    student_lines += student_entries;

    CHECK_LINES(student_lines, "with_hidden_dir\n")

    return 0;
}
