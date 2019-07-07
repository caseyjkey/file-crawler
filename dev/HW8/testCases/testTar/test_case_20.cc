#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("tree");

    multiset<string> perms;
    for (auto fp : b) {
        perms.insert(string(fp->perms()));
    }

    string perms_lines;
    perms_lines += perms;
 
    CHECK_LINES(perms_lines, "-r--r--r--\n"
                             "-r--r--r--\n"
                             "-rw-r--r--\n"
                             "-rw-r--r--\n"
                             "-rwxr--r-x\n"
                             "dr-xr-xr-x\n"
                             "drwxr-xr--\n"
                             "drwxr-xr-x\n"
                             "drwxr-xr-x\n")

    return 0;
}
