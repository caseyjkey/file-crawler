#include <iostream>

class Foo {
  private:
    static void *operator new(std::size_t);
    static void operator delete(void *, std::size_t);
};

using namespace std;

int main() {
    Foo bar;
    Foo *p = &bar;
    p = new Foo;
    cout << "done\n";
    return 0;
}
