// The assignment to f[14] is invalid, but it still seems to run just fine.
// Modify operator[] in fix.h to deal with this by
// printing a message to standard error and exiting in that case.

#include "fix.h"

int main() {
    fix<double, 13> f;
    f[14] = 6.66;
}
