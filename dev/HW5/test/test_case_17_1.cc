#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("simple");

    string student_lines = b.entry(0).mtime();
    student_lines += "\n";

    CHECK_LINES(student_lines, "2019-03-28T08:48:22\n")

    return 0;
}
