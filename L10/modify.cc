#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
	cerr << "usage: " << argv[0] << " file\n";
	return 1;
    }
    ifstream in(argv[1]);
    if (!in) {
	cerr << argv[0] << ": can't open " << argv[1] << '\n';
	return 1;
    }

    string s;
    while (getline(in, s)) {
	size_t n;
	while ( (n=s.find("global")) != string::npos )
	    s.replace(n, 6, "denial");
	in.close();
        ofstream out(argv[1]);
	out << s << '\n';
        out.close();
        ifstream in(argv[1]);
    }

    return 0;
}
