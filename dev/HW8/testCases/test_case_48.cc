#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    const Bunch b("tree2");

    EXPECT(b.begin() != b.end())
    auto it = b.end();
    EXPECT(b.end() == it--)

    it = b.begin();
    it++;
    EXPECT(b.begin() == --it)

    return 0;
}
