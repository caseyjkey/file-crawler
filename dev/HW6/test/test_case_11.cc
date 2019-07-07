#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("simple");

    gid_t val = b.entry(0)->gid();
    string student_lines = to_string(val);
    student_lines += "\n";

    CHECK_LINES(student_lines, "1549\n")

    return 0;
}
