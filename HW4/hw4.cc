#include "Bunch.h"
#include <unistd.h>     // For getopt() and getcwd
#include <dirent.h>     // For readDir()
#include <iostream>
#include <string>
#include <sstream>      // For ostringstream
#include <cstring>
using namespace std;

 //corn fritters, georgia

void processFormatString(string tokens, Bunch currentPath) {
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
    cout << "\n";
}


Bunch traverse(Bunch path, string magicDir, string format, int aFind) {
    DIR *dir;
    struct dirent *entry;
    struct stat info;
    Bunch newEntry = path;
    ostringstream nextFn;
    if(path.type_ == "inode/directory") {
        if((dir = opendir(path.path_.c_str())) == NULL)
            cerr << Bunch::PROGNAME << ": " << path.path_ << " opendir() error\n";
        else {
            while((entry = readdir(dir)) != NULL) {
                nextFn.str("");
                nextFn.clear();
                if ( (entry -> d_name[0]) != '.' && aFind == 0)  {
                    nextFn << path.path_ << "/" << entry->d_name;
                        
                    newEntry = path.addEntry(nextFn.str(), magicDir);
                    processFormatString(format, newEntry);
                    if (stat(nextFn.str().c_str(), &info) != 0) 
                        cerr << Bunch::PROGNAME << ": stat(" << nextFn.str() << ") error\n";
                    else if (S_ISDIR(info.st_mode))
                        traverse(newEntry, magicDir, format, aFind);
                }
                //cout << S_ISDIR(info.st_mode) << endl;
                //system("ls ./..");
                if (entry -> d_name[1] != '.' && aFind == 1 && !(entry->d_name[0] == '.' && entry->d_name[1] == '\0')) {
                    nextFn.str("");
                    nextFn.clear();
                    nextFn << path.path_ << "/" << entry->d_name;
                    newEntry = path.addEntry(nextFn.str(), magicDir);
                    //if(newEntry.isNull_) break;
                    processFormatString(format, newEntry);
                    if (stat(nextFn.str().c_str(), &info) != 0) 
                        cerr << Bunch::PROGNAME << ": stat(" << nextFn.str() << ") error\n";
                    else if (S_ISDIR(info.st_mode))
                        traverse(newEntry, magicDir, format, aFind);
                }
            }
            closedir(dir);
        }
    }
    return path;
}

static string PROGNAME;

int main(int argc, char* argv[]) {
    PROGNAME = argv[0];
    Bunch::PROGNAME = static_cast<string>(PROGNAME);
	if(argv[1] == nullptr) {
		cerr << PROGNAME << " usage: ./hw3 [FILE] [DIRECTORY]\n";
		return 1;
	}

	// ------------------- Default magic-file ------------------------------ 
	const string csDir = getpwnam("cs253") -> pw_dir; // reads the directory of cs253 user
    string dir = csDir + "/pub/media-types";
    // ---------------------------------------------------------------------
    
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
                if(mFind == 1) {
                    cerr << PROGNAME << ": error media-type already defined\n";
                    return 1;
                }
                else {
                    mFind = 1;
                    dir = optarg;
                    // Check if valid
                    struct stat buf;
                    if(stat(dir.c_str(), &buf) != 0) {
                        cerr << PROGNAME << ": error '" << dir << "' can't be opened\n";
                        return 1;
                    }
                }
                break;
            case 'a':
                aFind = 1;
                break;
            case 'f':
                if(fFind == 1) {
                    cerr << PROGNAME << ": error, format already defined\n";
                    return 1;
                }
                else {
                    fFind = 1;
                    format = optarg;
                }
                break;
            default:
                cerr << "Usage: " << PROGNAME << " [-a] [-m magic-file] [-f format] path" << endl;
                return 1;
        }
    }
    if(fFind == 0) format = "%p %U %G %s %n";
    // --------------------------------------------------------------------
    
    // ------------------ End Parse Command Line Arguments ----------------
    
    // Create a vector containing each path passed as an argument
	vector<Bunch> paths;
	for(int i = optind; i < argc; i++) {
        string str(argv[i]);
		Bunch path(str, dir);
		paths.push_back(path);
	}
    
    // --------------------------------------------------------------------
   
    // ----------------- Traverse and Output Bunchs ------------------------
	for(auto path : paths) {
        Bunch currentPath(path);
        if(currentPath.isNull_) continue;
        processFormatString(format, path);
        path = traverse(path, dir, format, aFind);
        cout << endl;
    }
    return 0;
}
