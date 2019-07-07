#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("simple");

    time_t mtime = b.entry(0).mtime();
    string student_lines = to_string(mtime);
    student_lines += "\n";

    CHECK_LINES(student_lines, "1553784502\n")

    return 0;
}
