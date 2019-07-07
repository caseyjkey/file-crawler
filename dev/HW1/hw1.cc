#include <iostream>
#include <sys/types.h> 
#include <sys/stat.h>

using namespace std;


int main(int argc, char* argv[]) {
	string PROGNAME = argv[0];

	if(argv[1] == nullptr) {
		cerr << PROGNAME << " usage: ./hw1 [FILE] [DIRECTORY]\n";
		return 1;
	}
	// Object for lstat data
	struct stat statbuf;
	
	const char* directory[10] = {};
	for(int i = 1; i < argc; i++) {	
		directory[i - 1] = argv[i];
	}
	
	for(int i = 0; i < argc - 1; i++) {
		int result = lstat(directory[i], &statbuf);
		if (result != 0) {
			cerr << PROGNAME << ": " << directory[i] << " failed\n";
			continue;
		}
		if(S_ISDIR(statbuf.st_mode)) cout <<  "d";
		else if(S_ISLNK(statbuf.st_mode)) cout << "l";
		else if(S_ISREG(statbuf.st_mode)) cout << "-";
		else {
			cerr << PROGNAME << ": " <<  argv[i+1] << " is an undefined file type, wtf?!\n";
			continue;
		}
		cout << (statbuf.st_mode & S_IRUSR ? 'r' : '-');
		cout << (statbuf.st_mode & S_IWUSR ? 'w' : '-');
		cout << (statbuf.st_mode & S_IXUSR ? 'x' : '-');
		cout << (statbuf.st_mode & S_IRGRP ? 'r' : '-');
		cout << (statbuf.st_mode & S_IWGRP ? 'w' : '-');
		cout << (statbuf.st_mode & S_IXGRP ? 'x' : '-');
		cout << (statbuf.st_mode & S_IROTH ? 'r' : '-');
		cout << (statbuf.st_mode & S_IWOTH ? 'w' : '-');
		cout << (statbuf.st_mode & S_IXOTH ? 'x' : '-');
		
		cout << " " << statbuf.st_size;

		time_t fileTime = statbuf.st_mtime;
		auto timeval = localtime(&fileTime);
		char buf[32];
		strftime(buf, sizeof(buf), "%Y-%m-%dT%T", timeval);
		cout << " " << buf << " " << directory[i] << "\n";
	}
	return 0;
}
