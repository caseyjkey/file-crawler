#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("simple");

    uid_t uid = b.entry(0).uid();
    string student_lines = to_string(uid);
    student_lines += "\n";

    CHECK_LINES(student_lines, "1509\n")

    return 0;
}
