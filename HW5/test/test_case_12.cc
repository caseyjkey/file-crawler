#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("simple");

    string student_lines = b.entry(0).uid();
    student_lines += "\n";

    CHECK_LINES(student_lines, "cs253\n")

    return 0;
}
