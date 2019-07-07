#include "Bunch.h"
#include <sstream>    // For ostringstream
using namespace std;

Fing::Fing() { }

Fing::~Fing() {}

// Constructor
Fing::Fing(const string &path, struct stat &statbuf) : statbuf_(statbuf), path_(path)  { }

// Copy Constructor
Fing::Fing(const Fing & rhs) : path_(rhs.path_) { }

// ------------ Derived Classes --------------

string Regular::type() const {
	return "regular";
}

string Symlink::type() const {
	return "symlink";
}

string Directory::type() const {
	return "directory";
}

// Factory method credit to: https://bit.ly/2ZJYeqJ
shared_ptr<const Fing> Fing::makeFing(const string &path) {
	struct stat statbuf;
    int openFile = lstat(path.c_str(), &statbuf);
	if(openFile != 0) 
        throw "cannot access '" + path + "': No such file or directory\n";
	
    if(S_ISDIR(statbuf.st_mode)) {
		shared_ptr<const Fing> p(new Directory(path, statbuf));
		return p;
	}
    
    if(S_ISLNK(statbuf.st_mode)) {
		shared_ptr<const Fing> p(new Symlink(path, statbuf));
        return p;
    }
	
	shared_ptr<const Fing> p(new Regular(path, statbuf));
	return p;
}

// ----------- End of Derived Classes -------------

Fing &Fing::operator=(const Fing & rhs) {
    path_ = rhs.path_;
    
    return *this;
}

bool Fing::operator==(const Fing &rhs) const{
    return (rhs.statbuf_.st_dev == statbuf_.st_dev && rhs.statbuf_.st_ino == statbuf_.st_ino);
}

ostream &operator<<(ostream &stream, const Fing &val) {
    return stream << val.path();    
}
    
string Fing::path() const { 
    return path_;
}

Dual<string, int> Fing::perms() const { 
	return {permissions(), int(statbuf_.st_mode)}; 
}

Dual<string, uid_t> Fing::uid() const { 
	return {user_NAME(statbuf_.st_uid), statbuf_.st_uid}; 
}

Dual<string, gid_t> Fing::gid() const { 
	return {group_NAME(statbuf_.st_gid), statbuf_.st_gid}; 
}

size_t Fing::size() const { 
	return statbuf_.st_size; 
}

Dual<string, time_t> Fing::atime() const { 
	return {time(1, 0, 0), statbuf_.st_atime}; 
}

Dual<string, time_t> Fing::mtime() const { 
	return {time(0, 1, 0), statbuf_.st_mtime}; 
}

Dual<string, time_t> Fing::ctime() const { 
	return {time(0, 0, 1), statbuf_.st_ctime}; 
}


string Fing::user_NAME(uid_t uid) const {
    struct passwd *pwd;
    if((pwd = getpwuid(uid)) == NULL)
        throw "requested uid not found in user database\n";
    return pwd->pw_name;
}

string Fing::group_NAME(gid_t gid) const {
    struct group *grp;
    if((grp = getgrgid(gid)) == NULL)
        throw "requested gid not found in group database\n";
    return grp->gr_name;
}

string Fing::permissions() const {
    ostringstream os;
    if(S_ISDIR(statbuf_.st_mode)) os <<  "d";
    else if(S_ISLNK(statbuf_.st_mode)) os << "l";
    else if(S_ISREG(statbuf_.st_mode)) os << "-";
    os << (statbuf_.st_mode & S_IRUSR ? 'r' : '-');
    os << (statbuf_.st_mode & S_IWUSR ? 'w' : '-');
    os << (statbuf_.st_mode & S_IXUSR ? 'x' : '-');

    os << (statbuf_.st_mode & S_IRGRP ? 'r' : '-');
    os << (statbuf_.st_mode & S_IWGRP ? 'w' : '-');
    os << (statbuf_.st_mode & S_IXGRP ? 'x' : '-');

    os << (statbuf_.st_mode & S_IROTH ? 'r' : '-');
    os << (statbuf_.st_mode & S_IWOTH ? 'w' : '-');
    os << (statbuf_.st_mode & S_IXOTH ? 'x' : '-');

    return os.str();
}

string Fing::time(bool access, bool mod, bool status) const {
    time_t fileTime;
    if(mod) {
        fileTime = statbuf_.st_mtime;
    }
    else if(access) {
        fileTime = statbuf_.st_atime;
    }
    else if(status) {
        fileTime = statbuf_.st_ctime;
    }
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%dT%T", localtime(&fileTime));
    return string(buf);
}