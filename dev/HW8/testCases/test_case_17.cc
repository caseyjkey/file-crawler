#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("tree");

    multiset<string> paths;
    for (size_t i = 0; i < b.size(); i++) {
        paths.insert(b.entry(i)->path());
    }

    string path_lines;
    path_lines += paths;
 
    CHECK_LINES(path_lines, "tree\n"
                            "tree/alpha\n"
                            "tree/mu\n"
                            "tree/theta\n"
                            "tree/zeta\n"
                            "tree/zeta/gamma\n"
                            "tree/zeta/gamma/epsilon\n"
                            "tree/zeta/gamma/kappa\n"
                            "tree/zeta/omega\n")

    return 0;
}
