#include "Bunch.h"
#include <iostream>           // For cerr
#include <sys/types.h>
#include <fstream>            // For ifstream
#include <sstream>            // For ostringstream
#include <dirent.h>           // For readDir()
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <pwd.h>
#include <bits/stdc++.h>
#include <string>
#include <grp.h>
using namespace std;

string Bunch::PROGNAME = "hw5";

// ---------------------- Constructors ---------------------------------
//Bunch::Bunch() { }

Bunch::Bunch(const string path) : Bunch(path, false) { }
Bunch::Bunch(const string path, bool all = false) {
            // Open a statbuf
            struct stat statbuf;
            // Check if the path is okay
            int openFile = lstat(path.c_str(), &statbuf);
            if(openFile != 0)
                throw PROGNAME + ": cannot access the path '" + path + "' thoe: No such file or directory\n";
            
            
            // Begin assigning values to attributes
            path_        = path;
            
            all_         = all;
            
            Fing newEntry = Fing(path_, all_);
            entries.push_back(newEntry);
            entryStrings.push_back(newEntry.path());

            
        
            traverse(path_);
            
            
}

Bunch::~Bunch() {}

// Copy constructor
Bunch::Bunch(const Bunch &rhs) : path_(rhs.path_), all_(rhs.all_), entries(rhs.entries), entryStrings(rhs.entryStrings) { }

Bunch &Bunch::operator=(const Bunch &rhs) {
    path_ = rhs.path_; 
    all_ = rhs.all_; 
    entries.clear();
    entries.push_back(Fing(path_, all_));
    traverse(path_);
    return *this;
}

// ---------------------------------------------------------------------

ostream &operator<<(ostream &stream, Bunch &val) {
    return stream << "empty?: " << val.empty();    
}


// ---------------------- Accessors and Mutators -----------------------
string Bunch::path(const string &path) { // replaces the path attribute of a Bunch, throw a string upon error including bad path
	struct stat statbuf;
	int openFile = lstat(path.c_str(), &statbuf);
	if(openFile != 0) 
		throw runtime_error(PROGNAME + ": cannot access the path '" + path + "': No such file or directory\n");
	
	path_ = path;
    traverse(path_);
    
    return path_;
}

void Bunch::all(bool all) { // default arg is true
    all_ = all;
    traverse(path_);
    
    return;
}
size_t Bunch::size() const { // number of entries
    return entries.size();
} 
bool Bunch::empty() const { //is entries == 0?
    return (size() == 0) ? true : false;
}
Fing Bunch::entry(size_t index) const {
    stringstream ss;
    if(index > entries.size()){
        ss << "expected number less than " << entries.size() << " but received " << index << "\n";
        throw ss.str();
    }
    return entries[index];
}



// ---------------------------- Traverse ----------------------------------

string Bunch::traverse(const string &directory) {
    DIR *dir;
    struct dirent *entry;
    struct stat info;
    
    ostringstream nextFilename;
    
    if((dir = opendir(directory.c_str())) != NULL) {
        while((entry = readdir(dir)) != NULL) {
            nextFilename.str("");
            nextFilename.clear();
            //Dont show a hidden file/directory
            if ( (entry -> d_name[0]) != '.' && !all_)  {
                
                nextFilename << directory << "/" << entry->d_name;
                
                Fing newEntry = Fing(nextFilename.str(), all_);
                entries.push_back(newEntry);
                entryStrings.push_back(newEntry.path());

                string newPath = entryStrings.back();
                
                if (stat(nextFilename.str().c_str(), &info) != 0) 
                    throw runtime_error(PROGNAME + ":Error, " + nextFilename.str() + " is not a valid file or directory\n");
                
                else if (S_ISDIR(info.st_mode))
                    traverse(nextFilename.str());
            }
            
            // Show hidden files and folders, but not dot directories
            if (entry -> d_name[1] != '.' && all_ && !(entry->d_name[0] == '.' && entry->d_name[1] == '\0')) {
                
                nextFilename << directory << "/" << entry->d_name;
                
                Fing newEntry = Fing(nextFilename.str(), all_);
                entries.push_back(newEntry);
                entryStrings.push_back(newEntry.path());

                string newPath = entryStrings.back();
                
                if (stat(nextFilename.str().c_str(), &info) != 0) 
                    throw runtime_error(PROGNAME + ":Error, " + nextFilename.str() + " is not a valid file or directory\n");
                
                else if (S_ISDIR(info.st_mode))
                    traverse(nextFilename.str());
                    
            }
        }
        
        
        closedir(dir);
    }
    
    return directory;
}



