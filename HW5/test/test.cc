#include "test.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <set>
#include <pwd.h>
#include <unistd.h>

#define REPORT_FILE_PATH "result"

using namespace std;

int main() {
    // Captures writes to standard streams.
    ostringstream out, err;
    auto stdout_buff = cout.rdbuf(out.rdbuf());
    auto stderr_buff = cerr.rdbuf(err.rdbuf());

    const string home = getpwnam("cs253")->pw_dir;
    chdir((home + "/pub/HW4_TEST_FILES"s).c_str());

    string info;
    int status = run_test_case(info);

    cout.rdbuf(stdout_buff);
    cerr.rdbuf(stderr_buff);

    if (out.str().length() != 0 || err.str().length() != 0) {
        report("Writes to stdout or stderr");
        return 1;
    }

    if (status != 0) {
        report(info);
        return 2;
    }

    report("PASS");
    return 0;
}

void report(const string &info) {
    // ofstream report(REPORT_FILE_PATH);
    // report << info << endl;
    cout << info << endl;
}

string& operator+=(string &s, const multiset<string> &sorted_lines) {
    for (const auto &line : sorted_lines) {
        s += line + '\n';
    }
    return s;
}

std::string truncated_if_too_long(const std::string &s) {
    if (s.length() <= MAX_STUDENT_LINES_LENGTH)
        return s;

    return s.substr(0, MAX_STUDENT_LINES_LENGTH) + "\n(truncated)\n"s;
}
