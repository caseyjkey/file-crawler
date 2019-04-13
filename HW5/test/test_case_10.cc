#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("simple");

    // string student_lines = b.entry(0);
    int perm = b.entry(0).perms(); 
    string student_lines = to_string(perm);
    student_lines += "\n";

    CHECK_LINES(student_lines, "33188\n")

    return 0;
}