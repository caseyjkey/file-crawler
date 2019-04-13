#include "Bunch.h"
#include "test.h"

using namespace std;
// Fing copy constructor.
int run_test_case(string &info) {
   	Fing b;
    info = "This should not compile. Bunch has no default constructor.";
    return 1;

}