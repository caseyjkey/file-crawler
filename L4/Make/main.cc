#include <iostream>

using namespace std;

#include "bar.h"
#include "foo.h"

int main() {
    cout << "Hi from " << __func__ << " at " __FILE__ ":" << __LINE__ << '\n';
    foo();
    bar();
    return 0;
}
