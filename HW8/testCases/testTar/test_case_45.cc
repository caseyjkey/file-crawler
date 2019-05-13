#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b("tree2/beta");

    for (auto fp : b) {
      if (fp->path() == "tree2/beta")
        EXPECT(dynamic_cast<const Directory *>(fp) != nullptr)
    }

    return 0;
}
