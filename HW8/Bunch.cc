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
#include <memory>             // For smart ptrs
using namespace std;

// ---------------------- Constructors ---------------------------------

Bunch::Bunch(const string &path) {            
            // Begin assigning values to attributes
            path_        = path;
            entries.push_back(Fing::makeFing(path_));
            traverse(path_);
}

// Dtor
Bunch::~Bunch() {
	/*
	cout << "Bunch: " << path() << "\n";
	for(auto &fing : entries) {
		cout << "Deleting: " << string(fing->perms()) << ' ' << fing->path() << endl;
		delete(fing);
		
	}
	entries.clear();
	for(auto entry : entries)
		cout << "Should be deleted: " << entry->path() << endl;
	cout << boolalpha << empty() << endl;
	*/
}

// Copy Ctor
Bunch::Bunch(const Bunch &rhs) : path_(rhs.path_) { 
	//cout << "Using copy ctor! ---------------------\n";
    entries = vector<shared_ptr<const Fing>>(rhs.entries.begin(), rhs.entries.end());
	//for(const auto &fing : rhs.entries) 
     //   entries.push_back(Fing::makeFing(fing->path()));
}

// ---------------------- Operators ------------------------------------

Bunch &Bunch::operator=(const Bunch &rhs) {
    path_ = rhs.path_; 
    entries.clear();
    for(const auto &fing : rhs.entries) 
        entries.push_back(Fing::makeFing(fing->path()));
    return *this;
}

// TODO: make statbuf an attribute of a fing
// Creating a set from vector: https://bit.ly/2Ivowaf
// while(inode < rhs.inode) check for if same
Bunch Bunch::operator+(const Bunch & rhs) const {
    Bunch freshBunch = *this;
    
    for(auto newFing : rhs.entries) {
        // Fing *copyOfNewFing = new Fing(*newFing); // New puts things in the heap 
		// unique_ptr<Fing> copyOfNewFing( new Fing(*newFing) );
        freshBunch.addEntry(Fing::makeFing(newFing->path()));
    }
    
    return freshBunch;
}

Bunch Bunch::operator-(const Bunch &rhs) const {
    Bunch freshBunch(*this);
    //Bunch freshBunch = Bunch(path_);  // This does not trigger the assignment operator    
    for(size_t i = 0; i < freshBunch.size(); i++) { // go by size of freshBunch
        for(const auto &rhsFing : rhs.entries) {
            if(*freshBunch.entries[i] == *rhsFing) {
                *freshBunch.entries.erase(freshBunch.entries.begin() + i);
            }
        }
    }
    return freshBunch;
}

Bunch &Bunch::operator+=(const Bunch &rhs) {
    for(const auto &newFing : rhs.entries) addEntry(Fing::makeFing(newFing->path()));
    return *this;
}

Bunch &Bunch::operator-=(const Bunch &rhs) {
    for(size_t i = 0; i < size(); i++)
        for(const auto &rhsFing : rhs.entries)
            if(*entries[i] == *rhsFing) 
                entries.erase(entries.begin() + i);
    return *this;
}

bool Bunch::operator==(const Bunch &rhs) const {
    bool fingFound = false;
    if(size() == rhs.size()) {
        for(const auto &fing1 : entries) {
            for(const auto &fing2 : rhs.entries) {
                if(*fing1 == *fing2) { 
                    fingFound = true;
                    break;
                }
            }
            if(fingFound) continue;
            else break;
        }
    }
    return fingFound;
}

bool Bunch::operator!=(const Bunch & rhs) const {
    return (*this == rhs) ? false : true;
}

Bunch::operator bool() const { 
    return !empty(); 
    
}

ostream &operator<<(ostream &stream, const Bunch &val) {
    if(!val.empty()) for(size_t i = 0; i < val.size(); i++) stream << *val.entry(i) << "\n";
    else stream << "Bunch empty";
    return stream;   
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
        ss << "expected 0 <= index <= " << entries.size() << " but received index " << index << "\n";
        throw ss.str();
    }
    
    return entries[index].get();
}

string Bunch::path() const {
    return path_;
}

bool Bunch::addEntry(shared_ptr<const Fing> newFing) {
    bool fingFound = false;
    
    for(const auto &fing : entries) {
		
		//cout << "\nTypes: " << typeid(fing).name() << " == " << typeid(newFing).name();
		//cout << boolalpha << (*fing == *newFing) << "\n";
        if(*fing == *newFing) { 
            fingFound = true;
            break;
        }
    }

    if(!fingFound) entries.push_back(newFing); // Possibly change to new Fing(*newFing)
    return !fingFound;
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
            nextFilename.str("");
            nextFilename.clear();
            //Dont show a hidden file/directory
            if ( (entry -> d_name[0]) != '.')  {
                
                nextFilename << directory << "/" << entry->d_name;

                entries.push_back(Fing::makeFing(nextFilename.str()));
                
                if (lstat(nextFilename.str().c_str(), &info) != 0) 
                    cerr << "Error, " + nextFilename.str() + " is not a valid file or directory\n";
                
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
    entries.push_back(Fing::makeFing(path_));
    traverse(path_);
    return;
}

