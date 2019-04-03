#include <stdexcept> # used for runtime error

using namespace std;

string Fing::PROGNAME = "hw5";

Fing::Fing() : Fing::Fing("") { }

Fing::~Fing() {}

Fing::Fing(const string path) {
    path_ = path;
    int openFile = lstat(path.c_str(), &statbuf);
    if(openFile != 0) 
        throw runtime_error(PROGNAME + ": cannot access the magic file '" + path + "': No such file or directory\n");
}

Fing::Fing(const Fing & rhs) : path_(rhs.path_) { }

Fing &Fing::operator=(const Fing & rhs) {
    path_ = rhs.path_;
}
    
string Fing::path() { 
    return path_;
}

void Fing::perms() const {
    
}

void Fing::uid() const {
    
}

void Fing::gid() const {
    
}

size_t Fing::size() const {      // number of entries
    return statbuf.st_size;
}

void Fing::atime() const {
    return time(statbuf, 1, 0, 0);
}

void Fing::mtime() const {
    return time(statbuf, 0, 1, 0);
}

void Fing::ctime() const {
    return time(statbuf, 0, 0, 1);
}
