#include "Bunch.h"
#include <sstream>    // For ostringstream

using namespace std;

Fing::Fing() { }

Fing::~Fing() {}

Fing::Fing(const string &path) {
	struct stat statbuf;
    int openFile = lstat(path.c_str(), &statbuf);
    if(openFile != 0) 
        throw "cannot access '" + path + "': No such file or directory\n";
    
    path_ = path;
    
}

Fing::Fing(const Fing & rhs) : path_(rhs.path_) { }

Fing &Fing::operator=(const Fing & rhs) {
    path_ = rhs.path_;
    
    return *this;
}

bool Fing::operator==(const Fing & rhs) const{
    struct stat statbuf;
    struct stat statbufRhs;
    lstat(path().c_str(), &statbuf);
    lstat(rhs.path().c_str(), &statbufRhs);

    return (statbufRhs.st_dev == statbuf.st_dev && statbufRhs.st_ino == statbuf.st_ino);
}

ostream &operator<<(ostream &stream, Fing &val) {
    return stream << val.path();    
}
    
string Fing::path() const { 
    return path_;
}

Dual<string, int> Fing::perms() const { 
	struct stat statbuf;
	lstat(path_.c_str(), &statbuf);
	return {permissions(), int(statbuf.st_mode)}; 
}

Dual<string, uid_t> Fing::uid() const { 
	struct stat statbuf;
	lstat(path_.c_str(), &statbuf);
	return {user_NAME(statbuf.st_uid), statbuf.st_uid}; 
}

Dual<string, gid_t> Fing::gid() const { 
	struct stat statbuf;
	lstat(path_.c_str(), &statbuf);
	return {group_NAME(statbuf.st_gid), statbuf.st_gid}; 
}

size_t Fing::size() const { 
	struct stat statbuf;
	lstat(path_.c_str(), &statbuf);
	return statbuf.st_size; 
}

Dual<string, time_t> Fing::atime() const { 
	struct stat statbuf;
	lstat(path_.c_str(), &statbuf);
	return {time(1, 0, 0), statbuf.st_atime}; 
}

Dual<string, time_t> Fing::mtime() const { 
	struct stat statbuf;
	lstat(path_.c_str(), &statbuf);
	return {time(0, 1, 0), statbuf.st_mtime}; 
}

Dual<string, time_t> Fing::ctime() const { 
	struct stat statbuf;
	lstat(path_.c_str(), &statbuf);
	return {time(0, 0, 1), statbuf.st_ctime}; 
}


string Fing::user_NAME(uid_t uid) const {
    struct passwd *pwd;
    if((pwd = getpwuid(uid)) == NULL)
        throw "requested uid not found in user database\n";
    return pwd->pw_name;
}

string Fing::group_NAME(gid_t gid) const {
    struct group *grp;
    if((grp = getgrgid(gid)) != NULL)
        throw "requested gid not found in group database\n";
    return grp->gr_name;
}

string Fing::permissions() const {
	struct stat statbuf;
	lstat(path_.c_str(), &statbuf);
    ostringstream os;
    if(S_ISDIR(statbuf.st_mode)) os <<  "d";
    else if(S_ISLNK(statbuf.st_mode)) os << "l";
    else if(S_ISREG(statbuf.st_mode)) os << "-";
    //else throw runtime_error(PROGNAME + ": " + path_ + " is an undefined file type, wtf?!\n");
    os << (statbuf.st_mode & S_IRUSR ? 'r' : '-');
    os << (statbuf.st_mode & S_IWUSR ? 'w' : '-');
    os << (statbuf.st_mode & S_IXUSR ? 'x' : '-');

    os << (statbuf.st_mode & S_IRGRP ? 'r' : '-');
    os << (statbuf.st_mode & S_IWGRP ? 'w' : '-');
    os << (statbuf.st_mode & S_IXGRP ? 'x' : '-');

    os << (statbuf.st_mode & S_IROTH ? 'r' : '-');
    os << (statbuf.st_mode & S_IWOTH ? 'w' : '-');
    os << (statbuf.st_mode & S_IXOTH ? 'x' : '-');

    return os.str();
}

string Fing::time(bool access = 0, bool mod = 1, bool status = 0) const {
	struct stat statbuf;
	lstat(path_.c_str(), &statbuf);
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
