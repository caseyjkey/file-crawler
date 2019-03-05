#include "path.h"
#include <unistd.h>     // For getopt() and getcwd
#include <dirent.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

 //corn fritters, georgia

void processFormatString(string tokens, Path currentPath) {
    for(string::size_type i = 0; i < tokens.size(); ++i) {
        if( tokens[i] != '%') {
                    cout << tokens[i];
                }
        else {
            ++i;
            if(tokens[i] == 'n') {
                cout << currentPath.path_;
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


Path traverse(Path path, string magicDir, string format) {
    DIR *dir;
    struct dirent *entry;
    struct stat info;
    
    ostringstream nextFn;
    //cout << "path type: " << path.type_;
    if(path.type_ == "inode/directory") {
        //cout << "is a directory\n";
        if((dir = opendir(path.path_.c_str())) == NULL)
            cerr << Path::PROGNAME << ": " << path.path_ << " opendir() error\n";
        else {
            while((entry = readdir(dir)) != NULL) {
                nextFn.str("");
                nextFn.clear();
                //cout << "inside while\n";
                if((entry -> d_name[0]) != '.') {
                    //cout << "inside d_name\n";
                    nextFn << path.path_ << "/" << entry->d_name;
                    
                    Path newEntry = path.addEntry(nextFn.str(), magicDir);
                    cout << "\n";
                    processFormatString(format, newEntry);
                    //cout << "\nnewEntry path: " << newEntry.path_ << "\n";
                    if (stat(nextFn.str().c_str(), &info) != 0) 
                        cerr << Path::PROGNAME << ": stat(" << nextFn.str() << ") error\n";
                    else if (S_ISDIR(info.st_mode))
                        traverse(Path(nextFn.str(), magicDir), magicDir, format);
                }
                
            }
        }
    }
    return path;
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

	// ---------------- Parse command line options -------------------------
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
    if(fFind == 0) format = "%p %U %G %s %n";
    
    
    cout << "aFind: " << aFind << " optind: " << optind << "\n";
    //cout << "\nformat: " << format << " magicFile: " << magicFile << "\n";
    // ------------------ End Parse Command Line Arguments ----------------
    
    // Create an array containing each directory/file
	vector<Path> paths;
   // char cwd[PATH_MAX];
	for(int i = optind; i < argc; i++) {
        string str(argv[i]);
		Path path(str, dir);
		paths.push_back(path);
	}
    
   
    
	for(auto path : paths) {
        Path currentPath(path);
        if(currentPath.isNull_) continue;
        processFormatString(format, path);
        path = traverse(path, dir, format);
        //cout << "\npath.entries size: " << path.entries.size() << "\n";
        cout << endl;
    }
    return 0;
}
