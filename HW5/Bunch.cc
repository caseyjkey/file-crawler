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


int Bunch::user_UID(struct stat & statbuf) {
    // https://ibm.co/2GwdIIR
    // Option to access via stat
    return static_cast<int>(statbuf.st_uid);
}
string Bunch::user_NAME(int uid) {
    struct passwd *pwd;
    if((pwd = getpwuid(uid)) != NULL)
        return pwd->pw_name;
    else
        throw runtime_error(PROGNAME + ": " + uid + " not found in user database\n");
    return "uid not found";
}
int Bunch::group_UID(struct stat &statbuf) {
    return statbuf.st_gid;
}
string Bunch::group_NAME(int gid) {
    struct group *grp;
    if((grp = getgrgid(gid)) != NULL)
        return grp->gr_name;
    else
        throw runtime_error(PROGNAME + ": " + gid + " not found in group database\n");
    return "gid not found";
}
int Bunch::permissions(struct stat &statbuf, string &output) {
    ostringstream os;
    if(S_ISDIR(statbuf.st_mode)) os <<  "d";
    else if(S_ISLNK(statbuf.st_mode)) os << "l";
    else if(S_ISREG(statbuf.st_mode)) os << "-";
    else throw runtime_error(PROGNAME + ": " + path_ + " is an undefined file type, wtf?!\n");
    os << (statbuf.st_mode & S_IRUSR ? 'r' : '-');
    os << (statbuf.st_mode & S_IWUSR ? 'w' : '-');
    os << (statbuf.st_mode & S_IXUSR ? 'x' : '-');

    os << (statbuf.st_mode & S_IRGRP ? 'r' : '-');
    os << (statbuf.st_mode & S_IWGRP ? 'w' : '-');
    os << (statbuf.st_mode & S_IXGRP ? 'x' : '-');

    os << (statbuf.st_mode & S_IROTH ? 'r' : '-');
    os << (statbuf.st_mode & S_IWOTH ? 'w' : '-');
    os << (statbuf.st_mode & S_IXOTH ? 'x' : '-');
    output = os.str();
    return 0;
}
int Bunch::sizeOfPath(struct stat &statbuf) {
    return statbuf.st_size;
}
string Bunch::time(struct stat &statbuf, bool access = 0, bool mod = 1, bool status = 0) {
    time_t fileTime;
    if(mod) {
        fileTime = statbuf.st_mtime;
    }
    else if(access) {
        fileTime = statbuf.st_atime;
    }
    else if(status) {
        fileTime = statbuf.st_ctime;
    }
    auto timeval = localtime(&fileTime);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%dT%T", timeval);
    string timeOutput(buf);
    return timeOutput;
}


string Bunch::inttohex(int num) {
    // https://bit.ly/2InTEd9
    string d = "0123456789abcdef"; //
    string res;
    if (num < 0) { 
        stringstream ss;
        ss << hex << num;
        string res = ss.str();
        int x = 0;
        for(unsigned int i = 0; i < res.length(); i++)
            if(res[i] == 'f') x++;
        string res1 = res.substr(x, res.length());
        return res1;
    }
    while(num > 0) {
        res = d[num % 16] + res;
        num /= 16;
    }
    if(res.length() == 1) res = "0" + res;
    return res;
}


