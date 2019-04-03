// #include "Bunch.h"
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

// ------------------- Default magic-file ------------------------------ 
const string csDir = getpwnam("cs253") -> pw_dir; // reads the directory of cs253 user
const string magicFile = csDir + "/pub/media-types";
// ---------------------------------------------------------------------

// ---------------------- Constructors ---------------------------------
Bunch::Bunch() : Bunch::Bunch("") { }

Bunch::Bunch(const string path, bool all = false) {
            // Open a statbuf
            struct stat statbuf;
            // Check if the magicFile is okay
            int openFile = lstat(magicFile.c_str(), &statbuf);
            if(openFile != 0) 
                throw runtime_error(PROGNAME + ": cannot access the magic file '" + path + "': No such file or directory\n");
                
            // Check if the path is okay
            openFile = lstat(path.c_str(), &statbuf);
            if(openFile != 0)
                throw runtime_error(PROGNAME + ": cannot access the path '" + path + "': No such file or directory\n");
            
            
            // Begin assigning values to attributes
            path_        = path;
            
            all_         = all;
            
            access_time_ = time(statbuf, 1, 0, 0);
            mod_time_    = time(statbuf, 0, 1, 0);
            status_time_ = time(statbuf, 0, 0 ,1);
            user_UID_    = user_UID(statbuf);
            group_UID_   = group_UID(statbuf);
            user_NAME_   = user_NAME(user_UID_);
            group_NAME_  = group_NAME(group_UID_);
            permissions(statbuf, permissions_);
            
            //if (find(entryStrings.begin(), entryStrings.end(), processFormatString(*this)) == entryStrings.end()) {
                entryStrings.push_back(processFormatString(*this));
            //}
            this->entries.push_back(*this);
        
            traverse(path_, all);
            
            
}

Bunch::~Bunch() {}

// Copy constructor
Bunch::Bunch(const Bunch &rhs) : path_(rhs.path_), type_(rhs.type_), permissions_(rhs.permissions_), user_UID_(rhs.user_UID_), group_UID_(rhs.group_UID_), group_NAME_(rhs.group_NAME_), user_NAME_(rhs.user_NAME_), access_time_(rhs.access_time_), mod_time_(rhs.mod_time_), status_time_(rhs.status_time_), format_(rhs.format_), all_(rhs.all_), entries(rhs.entries), entryStrings(rhs.entryStrings), fileSize_(rhs.fileSize_) { }

Bunch &Bunch::operator=(const Bunch &rhs) {
    path_ = rhs.path_; 
    type_ = rhs.type_; 
    permissions_ = rhs.permissions_; 
    user_UID_ = rhs.user_UID_; 
    group_UID_ = rhs.group_UID_; 
    group_NAME_ = rhs.group_NAME_; 
    user_NAME_ = rhs.user_NAME_; 
    access_time_ = rhs.access_time_; 
    mod_time_ = rhs.mod_time_; 
    status_time_ = rhs.status_time_; 
    all_ = rhs.all_; 
    entries = rhs.entries; 
    entryStrings = rhs.entryStrings; 
    fileSize_ = rhs.fileSize_;
    return *this;
}

// ---------------------------------------------------------------------

ostream &operator<<(ostream &stream, const Bunch &val) {
    return stream << "path: " << val.path_ 
                  << "\nentries size: " << val.entryStrings.size()
                  << "\nfile size: " << val.fileSize_
                  << "\nperms: " << val.permissions_
                  
}


// ---------------------- Accessors and Mutators -----------------------
void Bunch::path(string path) { // replaces the path attribute of a Bunch, throw a string upon error including bad path
	struct stat statbuf;
	int openFile = lstat(path.c_str(), &statbuf);
	if(openFile != 0) 
		throw runtime_error(PROGNAME + ": cannot access the path '" + path + "': No such file or directory\n");
	
	this->path_ = path;
    traverse(path_, all_);
    
    return;
}

void Bunch::all(bool all = true) { // default arg is true
    this->all_ = all;
    traverse(path_, all_);
    
    return;
}
size_t Bunch::size() const { // number of entries
    return this->entries.size();
} 
bool Bunch::empty() const { //is entries == 0?
    return (this->size() == 0) ? true : false;
}
string Bunch::entry(size_t index){

    return "implement me";
}



// ---------------------------- Traverse ----------------------------------

string Bunch::traverse(string directory, bool all) {
    DIR *dir;
    struct dirent *entry;
    struct stat info;
    
    ostringstream nextFilename;
    
    if((dir = opendir(directory.c_str())) != NULL) {
        while((entry = readdir(dir)) != NULL) {
            nextFilename.str("");
            nextFilename.clear();
            //Dont show a hidden file/directory
            if ( (entry -> d_name[0]) != '.' && !all)  {
                
                nextFilename << directory << "/" << entry->d_name;
                
                Bunch newEntry = Bunch(nextFilename.str(), magicFile_, format_, all_);
                entries.push_back(newEntry);
                entryStrings.push_back(newEntry.path_;

                string newPath = entryStrings.back();
                
                if (stat(nextFilename.str().c_str(), &info) != 0) 
                    throw runtime_error(PROGNAME + ":Error, " + nextFilename.str() + " is not a valid file or directory\n");
                
                else if (S_ISDIR(info.st_mode))
                    newEntry = traverse(nextFilename.str(), all);
            }
            
            // Show hidden files and folders, but not dot directories
            if (entry -> d_name[1] != '.' && all && !(entry->d_name[0] == '.' && entry->d_name[1] == '\0')) {
                
                nextFilename << directory << "/" << entry->d_name;
               
                Bunch newEntry = Bunch(nextFilename.str(), magicFile_, format_, all_);
                
                entries.push_back(newEntry);
                entryStrings.push_back(newEntry.path_);
                
                string newPath = entryStrings.back();
                
                
                if (stat(nextFilename.str().c_str(), &info) != 0) 
                    throw runtime_error(PROGNAME + ":Error, " + nextFilename.str() + " is not a valid file or directory\n");
                
                else if (S_ISDIR(info.st_mode)) 
                    newEntry = traverse(newEntry, all);
                    
            }
        }
        
        
        closedir(dir);
    }
    
    return directory;
}



