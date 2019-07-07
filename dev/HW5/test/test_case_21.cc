#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b(".hidden_dir/.hidden");
    string student_lines = b.entry(0).path();
    student_lines += "\n";

    CHECK_LINES(student_lines, ".hidden_dir/.hidden\n")

    return 0;
}
