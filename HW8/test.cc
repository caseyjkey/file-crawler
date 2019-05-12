#include "Bunch.h"	// Do this first for best testing.
#include <sys/types.h>	// for getpwnam
#include <pwd.h>	// for getpwnam
#include <unistd.h>	// for chdir
#include <cassert>	// assert
#include <iostream>	// for cout, cerr
#include <iomanip>	// for setw
#include <string>	// What a mystery!
#include <chrono>   // For the stopwatch
#include <vector>   // For testing
#include <cstdlib>  // For parsing command line args
#include <sys/ioctl.h> // For window

using namespace std;



int jacksTest(bool quiet = false) {
	
	try {
		
		auto dump = (quiet) ? [] (string label, const Bunch &b) { } : [] (string label, const Bunch &b) {
			cout << label << ":\n";
			for (auto p : b)
				cout << string(p->perms()) << ' ' << p->path() << endl;
			cout << '\n';
		};
		const auto home = getpwnam("cs253")->pw_dir; // cs253’s home dir
		if (chdir(home) != 0)			     // go to ~cs253
			throw "Can’t chdir to "s + home;
		const Bunch b1("pub/tree/alpha/iota");
		Bunch b2("pub/tree2/tau");
		dump("b1", b1);
        dump("b2", b2);
        dump("Bunch(/etc/group", Bunch("/etc/group"));
		// This causes memory leak
		b2 += Bunch("/etc/group");
		dump("b2 += Bunch(/etc/group)", b2);
		dump("b1+b2", b1+b2);
		Bunch b3(b2);
		
		b3 += b1;
		
		b3 += b1;				// Yes, I did it twice.
		
		b3 -= Bunch("/etc/resolv.conf");	// Should have no effect
		
		// No output is expected after this.  The assertions should all succeed.
		assert(b1.size() == 3);
		assert(b2.size() == 3);
		assert((b1+b2).size() == 6);
		dump("b3", b3);
		dump("b1+b2", b1+b2);
		assert(b1+b2 == b3);
		dump("b3", b3);
		dump("b2", b2);
        dump("b3-b2", b3-b2);
        dump("b1", b1);
		assert(b1 == b3-b2);
		assert(b1);
		assert(b2);
		assert(b3);
		assert(!(b1-b1));
		assert(!b1.empty());
		assert((b3 - b3).empty());
		
		
		Bunch::iterator it = b1.begin();
		assert(it != b1.end());
		const Fing *cfp = *it;
		assert(cfp != nullptr);
		assert(cfp->size() > 0);
		assert(b1.size() == 3);
		++it; assert(it != b1.end()); // it now “points” to the second entry
		it++; assert(b1.end() != it); // it now “points” to the third entry
		++it; assert(it == b1.end()); // it now “points” PAST the third entry
		
		const Bunch lotsafiles("pub/lotsafiles");
		for (const Fing *fp : lotsafiles) {
			const string perms = fp->perms(), type = fp->type();
			switch (perms[0]) {
			case 'd':
				assert(type == "directory");
				assert(dynamic_cast<const Directory *>(fp));
				assert(dynamic_cast<const Symlink *>(fp) == nullptr);
				break;
			case 'l':
				assert(type == "symlink");
				assert(dynamic_cast<const Symlink *>(fp));
				assert(dynamic_cast<const Regular *>(fp) == nullptr);
				break;
			case '-':
				assert(type == "regular");
				assert(dynamic_cast<const Regular *>(fp));
				assert(dynamic_cast<const Directory *>(fp) == nullptr);
				break;
			}
		}
		
		
		
	}
		
    catch (string err) {
		cerr << "ERROR: " << err << '\n';
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[]) {
	int reps = atoi(argv[argc - 1]);
	int startTime = 0;
	int finishTime = 0;
	vector<double> times;
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	cout << string(100, '\n') << "Progress: [";
	double inc = 0;
	int result = 0;
	for(int i = 0; i < reps; i++) {
		startTime = chrono::high_resolution_clock::now()
					.time_since_epoch().count();
		result = jacksTest(true);
		finishTime = chrono::high_resolution_clock::now()
					.time_since_epoch().count();
		times.push_back((finishTime - startTime) / 1e9);
		//cout << string((w.ws_col - 15) / reps, '#');
		inc += 100.0/reps;
		cout << string(100, '\n') << "Seconds: " << times.back() << "\nProgress: " << inc << "%\n";
		
		cout.flush();
	}
    cout << "\n";
	double total = 0;
	for(double time : times)
		total += time;
	cout << "Average: " << total/reps << endl;
	cout << "Success?: " << boolalpha << !bool(result) << endl;
	return 0;
}
