#include <iostream>
#include "Directory.h"

using std::cout;

int main() {
    Directory dir(".", "C.*");
    for (auto name : dir)
	cout << "Filename: " << name << '\n';
}
