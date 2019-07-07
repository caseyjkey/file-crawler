#include "Bunch.h"
#include "test.h"

using namespace std;

int run_test_case(string &info) {
    Bunch b1("tree");
    Bunch b2("tree2");
	auto dump = [] (string label, const Bunch &b) {
		cout << label << ":\n";
		for (auto p : b)
			cout << string(p->perms()) << ' ' << p->path() << endl;
		cout << '\n';
	};
	dump("b1", b1);
	dump("b2", b2);
    EXPECT(b1 != b2)

    return 0;
}
