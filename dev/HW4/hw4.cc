#include "Bunch.h"
#include <unistd.h>     // For getopt() and getcwd
#include <dirent.h>     // For readDir()
#include <iostream>
#include <string>

#include <cstring>
using namespace std;

 //corn fritters, georgia

void processFormatString(Bunch currentPath) {
    string tokens = currentPath.format_;
    for(unsigned int i = 0; i < tokens.size(); ++i) {
        
        if( tokens [i] == '%') {
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
        else if( tokens[i] == '\\') {
            //int j = i + 1;
            //ostringstream escape;
            //escape << tokens[i] << tokens[j];
            //char escapeChar = char(escape.str());
            i++;
            cout << "\t"; //escape.str();
        }
        else if( tokens[i] != '%') {
            cout << tokens[i];
        }
    }
    cout << "\n";
}


Bunch traverse(Bunch path, string magicDir, string format, bool all) {
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
                // Is the directory hidden and we dont want all?
                if ( (entry -> d_name[0]) != '.' && !all)  {
                    nextFn << path.path_ << "/" << entry->d_name;
                        
                    newEntry = path.addEntry(nextFn.str(), magicDir, format, all);
                    processFormatString(newEntry);
                    if (stat(nextFn.str().c_str(), &info) != 0) 
                        cerr << Bunch::PROGNAME << ": stat(" << nextFn.str() << ") error\n";
                    else if (S_ISDIR(info.st_mode))
                        traverse(newEntry, magicDir, format, all);
                }
                // We want all. Is it hidden or just a default dot directory?
                if (entry -> d_name[1] != '.' && all && !(entry->d_name[0] == '.' && entry->d_name[1] == '\0')) {
                    nextFn.str("");
                    nextFn.clear();
                    nextFn << path.path_ << "/" << entry->d_name;
                    newEntry = path.addEntry(nextFn.str(), magicDir, format, all);
                    processFormatString(newEntry);
                    if (stat(nextFn.str().c_str(), &info) != 0) 
                        cerr << Bunch::PROGNAME << ": stat(" << nextFn.str() << ") error\n";
                    else if (S_ISDIR(info.st_mode))
                        traverse(newEntry, magicDir, format, all);
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
    string magic = csDir + "/pub/media-types";
    // ---------------------------------------------------------------------
    
	// ---------------- Parse command line options -------------------------
	int opt;
	bool aFind, mFind, fFind;
	string magicFile, format;

    aFind = false;
    mFind = false;
    fFind = false;
	while((opt = getopt(argc, argv, "am:f:")) != -1) {
        switch (opt) {
            case 'm':
                if(mFind == 1) {
                    cerr << PROGNAME << ": error media-type already defined\n";
                    return 1;
                }
                else {
                    mFind = true;
                    magic = optarg;
                    // Did the user pass a valid magic file?
                    struct stat buf;
                    if(stat(magic.c_str(), &buf) != 0) {
                        cerr << PROGNAME << ": error '" << magic << "' can't be opened\n";
                        return 1;
                    }
                }
                break;
            case 'a':
                aFind = true;
                break;
            case 'f':
                if(fFind == 1) {
                    cerr << PROGNAME << ": error, format already defined\n";
                    return 1;
                }
                else {
                    fFind = true;
                    format = optarg;
                }
                break;
            default:
                cerr << "Usage: " << PROGNAME << " [-a] [-m magic-file] [-f format] path" << endl;
                return 1;
        }
    }
    if(!fFind) format = "%p %U %G %s %n";
    // --------------------------------------------------------------------
    
    // ------------------ End Parse Command Line Arguments ----------------
    
    // Create a vector containing each path passed as an argument
	vector<Bunch> paths;
	for(int i = optind; i < argc; i++) {
        string aPath(argv[i]);
		Bunch path(aPath, magic, format, aFind);
		paths.push_back(path);
	}
    
    // --------------------------------------------------------------------
   
    // ----------------- Traverse and Output Bunchs ------------------------
	for(auto path : paths) {
        //Bunch currentPath(path, magic, format, aFind);
        if(path.isNull_) continue;
        processFormatString(path);
        path = traverse(path, magic, format, aFind);
        cout << endl;
    }
    return 0;
}
