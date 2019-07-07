#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <set>

using namespace std;

int main() {
	vector<int> input;
	int in = 4;
	while (in != 0) {
		cin >> in;
		input.push_back(in);
	}
	for (auto elem : input) {
		cout << elem << " ";
	}
	cout << endl;

	ifstream f("/etc/resolv.conf");
	string stream;
	char c;
	while(f.get(c)) {
		stream.push_back(c);
	}
	multiset<char> ms;
	for(char ch : stream) {
		ms.insert(ch);
	}
	set<char> ss;
	for(char ch : ms) {
		ss.insert(ch);
	}
	cout << "String stream: size = " << stream.size() << ", content = \n" << stream
	     << "\n\nMultiset ms: size = " << ms.size() << ", content = ";
	for(auto out : ms) {
		cout << out;
	}

	cout << "\n\nSet ss: size = " << ss.size() << ", content = ";
	for(auto out : ss) {
		cout << out;
	}
	cout << endl;
	return 0;
}
