#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    multiset<string> entries;
    for (auto fp : Bunch("tree") + Bunch("tree2")) {
        entries.insert(string(fp->path()));
    }

    string path_lines;
    path_lines += entries;
 
    CHECK_LINES(path_lines, "tree\n"
                            "tree/alpha\n"
                            "tree/mu\n"
                            "tree/theta\n"
                            "tree/zeta\n"
                            "tree/zeta/gamma\n"
                            "tree/zeta/gamma/epsilon\n"
                            "tree/zeta/gamma/kappa\n"
                            "tree/zeta/omega\n"
                            "tree2\n"
                            "tree2/alpha\n"
                            "tree2/beta\n"
                            "tree2/beta/sigma\n"
                            "tree2/tau\n")

    return 0;
}
