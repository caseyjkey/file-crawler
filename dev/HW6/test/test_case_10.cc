#include "Bunch.h"
#include "test.h"


using namespace std;

int run_test_case(string &info) {
    //cout << "no one:\n" << endl;
    Bunch b("simple");

    string student_lines = b.entry(0)->gid();
    //cout << "after\n";
	student_lines += "\n";
    //cout << "student_lines" << student_lines;

    CHECK_LINES(student_lines, "class\n")

    return 0;
}
