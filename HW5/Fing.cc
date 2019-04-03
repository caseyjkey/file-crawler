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

