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
                throw "Cannot access the path '" + path + "': no such file or directory.\n";
            
            
            // Begin assigning values to attributes
            path_        = path;
            entries.push_back(Fing(path_));
        
            traverse(path_);
            
            
}

// Dtor
Bunch::~Bunch() {}

// Copy Ctor
Bunch::Bunch(const Bunch &rhs) : path_(rhs.path_), all_(rhs.all_), entries(rhs.entries) { }

// ---------------------- Operators ------------------------------------

Bunch &Bunch::operator=(const Bunch &rhs) {
    path_ = rhs.path_; 
    all_ = rhs.all_; 
    updatePath();
    return *this;
}

struct path_compare {
    bool operator() (const Fing& x, const Fing& y) const {
        return x.path_.compare(y.path_) < 0;
    }
};


//TODO: make statbuf an attribute of a fing
// Creating a set from vector: https://bit.ly/2Ivowaf
// while(inode < rhs.inode) check for if same
Bunch Bunch::operator+(const Bunch & rhs) const {
    /*set<Fing, path_compare> s;
    
    unsigned size = entries.size();
    for( unsigned i = 0; i < size; ++i ) s.insert( entries[i] );
    
    size = rhs.entries.size();
    for( unsigned i = 0; i < size; ++i ) s.insert( rhs.entries[i] );
    */
    Fing returnFing = *this;
    
    for(const auto &newFing : rhs.entries) {
        returnFing.addEntry(newFing);
    }
    
    return returnFing;
}
Bunch Bunch::operator-(const Bunch &) const;

Bunch Bunch::operator+=(const Bunch &) const;
Bunch Bunch:::operator-=(const Bunch &) const;


// Can number of fings be different, like one be a subset?
bool operator==(const Bunch & rhs) const {
    if(this.size() != rhs.size()) return false;

    bool fingFound = false;
    
    for(const auto &fing1 : entries) {
        for(const auto &fing2 : rhs.entries) {
            if(fing1 == fing2) { 
                fingFound = true;
                break;
            }
        }
        if(fingFound) continue;
        else break;
    }
    
    return fingFound;
}
bool operator!=(const Bunch & rhs) const {
    return (this == rhs) ? return false : return false;
}

explicit operator bool() const { return empty(); }

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
    stringstream ss;
    if(index > entries.size()){
        ss << "expected number less than " << entries.size() << " but received " << index << "\n";
        throw ss.str();
    }
    
    return &entries[index];
}

string Bunch::path() const {
    return path_;
}

bool Bunch::addEntry(const Fing &newFing) {
    // Check if this entry exists
    for(const auto &oldFing : entries) {
            if() 
                entries.push_back(newFing);
            else return false;
    }
    // Successfully added the Fing to entries
    return true;
}
// ------------------------------------------------------------------------


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
// ---------------------------------------------------------------------

// updatePath: clears out old entries and traverses
void Bunch::updatePath() {
    entries.clear();
    entries.push_back(Fing(path_));
    traverse(path_);
    return;
}

