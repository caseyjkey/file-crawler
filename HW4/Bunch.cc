#include "Bunch.h"
#include <iostream>           // For cerr
#include <sys/types.h>
#include <fstream>            // For ifstream
#include <dirent.h>           // For readDir()
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <pwd.h>
#include <bits/stdc++.h>
#include <string>
#include <grp.h>
using namespace std;

string Bunch::PROGNAME = "hw4";

// ------------------- Default magic-file ------------------------------ 
const string csDir = getpwnam("cs253") -> pw_dir; // reads the directory of cs253 user
const string magicFile = csDir + "/pub/media-types";
// ---------------------------------------------------------------------

// ---------------------- Constructors ---------------------------------
Bunch::Bunch() : Bunch::Bunch("") { }

Bunch::Bunch(const string path) : Bunch::Bunch(path, magicFile) { }

Bunch::Bunch(const string path, const string magicFile) : Bunch::Bunch(path, magicFile, "%p %U %G %s %n") { }

Bunch::Bunch(const string path, const string magicFile, const string format) : Bunch::Bunch(path, magicFile, format, false) { } 

Bunch::Bunch(const string path, const string magicFile, const string format, bool all = false) {
            // Open a statbuf
            struct stat statbuf;
            int openFile = lstat(path.c_str(), &statbuf);
            if(openFile != 0) {
                cerr << PROGNAME << ": cannot access '" << path << "': No such file or directory\n";
                isNull(true);
                return;
            }
            
            // Begin assigning values to attributes
            fileSize_    = statbuf.st_size;
            mediaTypes   = readMediaTypeFile(magicFile);
            path_        = path;
            access_time_ = time(statbuf, 1, 0, 0);
            mod_time_    = time(statbuf, 0, 1, 0);
            status_time_ = time(statbuf, 0, 0 ,1);
            user_UID_    = user_UID(statbuf);
            group_UID_   = group_UID(statbuf);
            user_NAME_   = user_NAME(user_UID_);
            group_NAME_  = group_NAME(group_UID_);
            magic_num_   = readMagicNumber(path_);
            type_        = findMediaType(magic_num_, mediaTypes, statbuf);
            format_      = format;
            all_         = all;
            permissions(statbuf, permissions_);
            traverse(*this, magicFile, format, all);
            
}
// ---------------------------------------------------------------------

// ---------------------- Accessors and Mutators -----------------------
void Bunch::path(string path) { // replaces the path attribute of a Bunch, throw a string upon error including bad path
	struct stat statbuf;
	int openFile = lstat(path.c_str(), &statbuf);
	if(openFile != 0) {
		cerr << PROGNAME << ": cannot access '" << path << "': No such file or directory\n";
		isNull(true);
		return;
	}
}
void Bunch::magic(string) { // Same rules as above regarding errors

}	
void Bunch::format(string format = "%p %U %G %s %n") {  // default arg is %p %U %G %s %n
	this->format_ = format;
}
void Bunch::all(string) { // default arg is true

}
size_t Bunch::size() const { // number of entries

} 
bool Bunch::empty() const { //is entries == 0?

}
string Bunch::entry(size_t index) const {

}

// -------------------- Build Entries ----------------------------------
Bunch Bunch::traverse(Bunch bunch, string magicDir, string format, bool all) {
    DIR *dir;
    struct dirent *entry;
    struct stat info;
    Bunch newEntry = bunch;
    ostringstream nextFn;
    if(bunch.type_ == "inode/directory") {
        if((dir = opendir(bunch.path_.c_str())) == NULL)
            cerr << Bunch::PROGNAME << ": " << bunch.path_ << " opendir() error\n";
        else {
            while((entry = readdir(dir)) != NULL) {
                nextFn.str("");
                nextFn.clear();
                // Is the directory hidden and we dont want all?
                if ( (entry -> d_name[0]) != '.' && !all)  {
                    nextFn << bunch.path_ << "/" << entry->d_name;
                    newEntry = bunch.addEntry(nextFn.str(), magicDir, format, all);
                    if (stat(nextFn.str().c_str(), &info) != 0) 
                        cerr << Bunch::PROGNAME << ":Error, " << nextFn.str() << " is not a valid file or directory\n";
                    else if (S_ISDIR(info.st_mode))
                        traverse(newEntry, magicDir, format, all);
                }
                // We want all. Is it hidden or just a default dot directory?
                if (entry -> d_name[1] != '.' && all && !(entry->d_name[0] == '.' && entry->d_name[1] == '\0')) {
                    nextFn << bunch.path_ << "/" << entry->d_name;
                    newEntry = bunch.addEntry(nextFn.str(), magicDir, format, all);
                    if (stat(nextFn.str().c_str(), &info) != 0) 
                        cerr << Bunch::PROGNAME << ": stat(" << nextFn.str() << ") error\n";
                    else if (S_ISDIR(info.st_mode))
                        traverse(newEntry, magicDir, format, all);
                }
            }
            closedir(dir);
        }
    }
    return bunch;
}

Bunch &Bunch::addEntry(string path, string magicFile, string format, bool all) {
    Bunch newBunch(path, magicFile, format, all);
    entries.push_back(newBunch);
    return this->entries.back();
}

// ---------------------------------------------------------------------


vector< pair<string, string> > Bunch::readMediaTypeFile(string dir) {
    ifstream inFile;
    inFile.open(dir);
    string token;
    pair<string, string> mediaEntry;
    while(inFile >> token) {
        mediaEntry.first = token;
        inFile >> token;
        mediaEntry.second = token;
        mediaTypes.push_back(mediaEntry);
    }
    return mediaTypes;
}

void Bunch::isNull(bool setTo) {
    isNull_ = setTo;
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
        cerr << PROGNAME << ": " << uid << " not found in user database\n";
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
        cerr << PROGNAME << ": " << gid << " not found in group database\n";
    return "gid not found";
}
int Bunch::permissions(struct stat &statbuf, string &output) {
    ostringstream os;
    if(S_ISDIR(statbuf.st_mode)) os <<  "d";
    else if(S_ISLNK(statbuf.st_mode)) os << "l";
    else if(S_ISREG(statbuf.st_mode)) os << "-";
    else {
        cerr << PROGNAME << ": " << path_ << " is an undefined file type, wtf?!\n";
        return 1;
    }
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
int Bunch::sizePath(struct stat &statbuf) {
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
    
    //if(num == 127) num = 137;
    //cout << "Base 16 num: " << num << " ";
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
    //cout << "Decimal result: " << res << "\n";
    return res;
}

string Bunch::readMagicNumber(string dir) {
    ifstream file(dir);
    char chrctr;
    string magicNum;
    for ( int i = 0; i < 32; i++ ) {
        file.get(chrctr);
        if(chrctr < '!' || chrctr > '~') {
            magicNum += "%" +  inttohex(chrctr);
        } else {
            magicNum += chrctr;
        }
    }
    //cout << "magicNum: " << magicNum << "\n";
    return magicNum;
}

string Bunch::findMediaType(string magicNum, vector< pair<string, string> > MediaTypes, struct stat &statbuf) {
    if(S_ISDIR(statbuf.st_mode)) return "inode/directory";
    if(S_ISLNK(statbuf.st_mode)) return "inode/symlink";
    if(sizePath(statbuf) == 0) return "inode/empty";
    bool match;
    for(auto& elem : MediaTypes) {
        match = 1;
        for(unsigned int i = 0; i < elem.first.length(); i++) {
                    //cout << magicNum[i] << " compared to " << elem.first[i] << "\n";
            if(magicNum[i] != elem.first[i]) {
                    match = 0;
            }
        }
        if(match) return elem.second;
    }
    return "application/octet-data";
}






