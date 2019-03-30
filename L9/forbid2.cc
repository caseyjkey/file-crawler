#include <iostream>

class Foo {
  public:
    static void *operator new(std::size_t) = delete;
    static void operator delete(void *, std::size_t) = delete;
};

using namespace std;

int main() {
    Foo bar;
    Foo *p = &bar;
    p = new Foo;
    cout << "done\n";
    return 0;
}
