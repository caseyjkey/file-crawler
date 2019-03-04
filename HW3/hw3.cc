#include "path.h"
#include <iostream>
#include <string>
#include <unistd.h>     // For getopt()
using namespace std;

 //corn fritters, georgia

vector<string> parseFormatString(string str, int len) {
    string tokens = string(argv[1]);
    for(string::size_type i = 0; i < tokens.size(); ++i) {
        if( tokens[i] != '%') {
                    cout << tokens[i];
                }
        else {
            ++i;
            if(tokens[i] == 'n') {
                cout << path.path_;
            }
            else if(tokens[i] == 'p') {
                cout << currentPath.permissions_;
            }
            else if(tokens[i] == 'u') {
                cout << currentPath.user_UID_;
            }
            else if(tokens[i] == 'U') {
                cout << currentPath.user_NAME_;
            }
            else if(tokens[i] == 'g') {
                cout << currentPath.group_UID_;
            }
            else if(tokens[i] == 'G') {
                cout << currentPath.group_NAME_;
            }
            else if(tokens[i] == 's') {
                cout << currentPath.size_;
            }
            else if(tokens[i] == 'a') {
                cout << currentPath.access_time_;
            }
            else if(tokens[i] == 'm') {
                cout << currentPath.mod_time_;
            }
            else if(tokens[i] == 'c') {
                cout << currentPath.status_time_;
            }
            else if(tokens[i] == 'M') {
                cout << currentPath.type_;
            }
        }
    }
}


        


// Start of HW3, TODO: Section out homeworks into seperate classes.
//void printContents(Path path) {
	//if(findMediaType(string magicNum, vector<
//}

static string PROGNAME;

int main(int argc, char* argv[]) {
    PROGNAME = argv[0];
    Path::PROGNAME = static_cast<string>(PROGNAME);
	if(argv[1] == nullptr) {
		cerr << PROGNAME << " usage: ./hw3 [FILE] [DIRECTORY]\n";
		return 1;
	}
	// Read the flags
	//vector<string> tokens = parseFormatString(argv[1], 1);

	// Determine the media types of files
	const string csDir = getpwnam("cs253") -> pw_dir; // reads the directory of cs253 user
    string dir = csDir + "/pub/media-types";

	
	int opt;
	int aFind, mFind, fFind;
	string magicFile, format;

    aFind = 0;
    mFind = 0;
    fFind = 0;
	while((opt = getopt(argc, argv, "am:f:")) != -1) {
        switch (opt) {
            case 'm':
                if(mFind == 1)
                    cerr << PROGNAME << ": error, media-type already defined." << endl;
                else {
                    mFind = 1;
                    magicFile = optarg;
                    cout << "magicFile: " << magicFile << endl;
                }
                break;
            case 'a':
                aFind = 1;
                break;
            case 'f':
                if(fFind == 1)
                    cerr << PROGNAME << ": error, format already defined." << endl;
                else {
                    fFind = 1;
                    format = optarg;
                    cout << "magicFile: " << magicFile << endl;
                }
                break;
            default:
                cerr << "Usage: " << PROGNAME << " [-a] [-m magic-file] [-f format] path" << endl;
                return 1;
        }
    }
    
    cout << "aFind: " << aFind << " mFind: " << mFind << " fFind: " << fFind << "\n";
    cout << "\nformat: " << format << " magicFile: " << magicFile << "\n";
    
    // Create an array containing each directory/file
	vector<Path> paths;
	for(int i = 2; i < argc; i++) {
        string str(argv[i]);
		Path path(argv[i], dir);
		paths.push_back(path);
	}
    
	for(auto path : paths) {
        Path currentPath(path);
        if(currentPath.isNull_) continue;

		
        cout << endl;
    }
    return 0;
}
