#include <iostream>
#include <cstring>		// C-strings: for strcpy()
using namespace std;

int main() {
    char *greeting = new char[1024];
    strcpy(greeting, "Dear");
    cout << greeting << " Sir/Madam\n";
    delete [] greeting; // had to delete after use
    return 0;
}
