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
#include <string>
#include <grp.h>
using namespace std;

// ---------------------- Constructors ---------------------------------

Bunch::Bunch(const string &path) {
            // Open a statbuf
            struct stat statbuf;
            // Check if the path is okay
            int openFile = lstat(path.c_str(), &statbuf);
            if(openFile != 0)
                throw "cannot access the path '" + path + "': No such file or directory\n";
            
            
            // Begin assigning values to attributes
            path_        = path;
            
            Fing newEntry = Fing(path_);
            entries.push_back(newEntry);
            entryStrings.push_back(newEntry.path());

            
        
            traverse(path_);
            
            
}

Bunch::~Bunch() {}

// Copy constructor
Bunch::Bunch(const Bunch &rhs) : path_(rhs.path_), all_(rhs.all_), entries(rhs.entries), entryStrings(rhs.entryStrings) { }

// ---------------------- Operators ------------------------------------

Bunch &Bunch::operator=(const Bunch &rhs) {
    path_ = rhs.path_; 
    all_ = rhs.all_; 
    return *this;
}


//TODO: make statbuf an attribute of a fing
Bunch Bunch::operator+(const Bunch & rhs) {
    
}
Bunch Bunch::operator-(const Bunch &);

Bunch Bunch::operator+=(const Bunch &);
Bunch Bunch:::operator-=(const Bunch &);


// Can number of fings be different, like one be a subset?
bool operator==(const Bunch & rhs) const {
    struct stat statbuf1;
    struct stat statbuf2;
    
    if(this.size() != rhs.size()) return false;
    
    for(size_t i = 0; i < entries.size(); i++) {
        Fing fing1 = entries[i];
        lstat(fing1.path().c_str(), statbuf1);
        for(size_t i = 0; i < entries.size(); i++) {
            Fing fing2 = rhs.entries[i];
            lstat(fing2.path().c_str(), statbuf2);
            if(statbuf2.st_dev != statbuf1.st_dev || statbuf2.st_ino != statbuf1.st_ino) 
                return false;
        }
    }
    
    return true;
}
bool operator!=(const Bunch & rhs) const {
    return (this == rhs) ? return false : return false;
}

bool operator bool() const { return empty(); }

ostream &operator<<(ostream &stream, Bunch &val) {
    return stream << "empty?: " << val.empty();    
}

// ---------------------------------------------------------------------

// ---------------------- Accessors and Mutators -----------------------

size_t Bunch::size() const { // number of entries
    return entries.size();
} 
bool Bunch::empty() const { //is entries == 0?
    return (size() == 0) ? true : false;
}
const Fing * Bunch::entry(size_t index) const {
    return &entries[index];
}

// ---------------------------- Traverse ----------------------------------

string Bunch::traverse(const string &directory) {
    DIR *dir;
    struct dirent *entry;
    struct stat info;
    
    ostringstream nextFilename;
    
    if((dir = opendir(directory.c_str())) != NULL) {
        while((entry = readdir(dir)) != NULL) {
            nextFilename.str("").clear();
            //Dont show a hidden file/directory
            if ( (entry -> d_name[0]) != '.' && !all_)  {
                
                nextFilename << directory << "/" << entry->d_name;
                
                Fing newEntry = Fing(nextFilename.str(), all_);
                entries.push_back(newEntry);
                entryStrings.push_back(newEntry.path());

                string newPath = entryStrings.back();
                
                if (stat(nextFilename.str().c_str(), &info) != 0) 
                    throw "Error, " + nextFilename.str() + " is not a valid file or directory\n";
                
                else if (S_ISDIR(info.st_mode))
                    traverse(nextFilename.str());
            }
        }
        closedir(dir);
    }
    
    return directory;
}



