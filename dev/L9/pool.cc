#include <iostream>

using namespace std;

class Bar {
  public:
    static void *operator new(size_t);
    static void operator delete(void *, size_t);
    double datum;
};

union Node {			// Not a class, not a struct: a union!
    Node *next;
    char pad[sizeof(Bar)];
};
static Node *freelist;

static void *Bar::operator new(size_t) {
    if (freelist == nullptr) {				// Out of free nodes?
	constexpr int NODES_PER_PAGE = 8192/sizeof(Node);  // A bunch of them
	Node *n = new Node[NODES_PER_PAGE];		// Allocate many
	// Link all the new free nodes into the freelist
	freelist = n;
	for (int i=0; i<NODES_PER_PAGE-1; i++, n++)
	    n->next = n+1;
	n->next = nullptr;
    }
    void *result = freelist;	// Get the first node
    freelist = freelist->next;	// Remove it from the free list
    return result;
}

static void Bar::operator delete(void *p, size_t) {
    if (!p) return;				// Pesky null pointer!
    // Return this node to the list of free nodes.
    Node *n = reinterpret_cast<Node *>(p);
    n->next = freelist;
    freelist = n;
}

int main() {
    cout << "sizeof(double) = " << sizeof(double) << '\n'
	 << "sizeof(Bar) = " << sizeof(Bar) << '\n';
    Bar *r = new Bar;
    Bar *s = new Bar;
    cout << "r=" << r << '\n'
	 << "s=" << s << '\n';
    delete r;
    delete s;
    return 0;
}
