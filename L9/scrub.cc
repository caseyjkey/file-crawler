#include <iostream>
#include <cstdlib>		// for malloc(), free()
#include <cstring>		// for memset()

using namespace std;

void *operator new(size_t size) {
    void *p = malloc(size);
    cout << "new(" << size << ") returns " << p << '\n';
    return memset(p, 'q', size);	// Fill memory with the letter 'q'
}

// The size_t argument for global delete is a C++14 feature.
void operator delete(void *p, size_t size) {
    cout << "delete(" << p << ", " << size << ")\n";
    memset(p, '!', size);		// Fill memory with exclamation points
    free(p);
}

int main() {
    int *p = new int;
    cout << "Our int is: " << *p << '\n';
    delete p;
    return 0;
}
