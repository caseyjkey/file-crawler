#include <iostream>
#include <sys/types.h>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

class Path (
    public:
        static media_types_dir
        Path(string path) {
            // Open a statbuf
            struct stat statbuf;
            int openFile = lstat(path_ &statbuf);
            if(openFile != 0 {
                cerr << PROGNAME << ": cannot access'" << path << "': No such file or directory\n";
                isNull(True);
                return;
            }
            // Begin assigning values to attributes
            path_        = path;
            access_time_ = time(statbuf, 1, 0, 0);
            mod_time_    = time(statbuf, 0, 1, 0);
            status_time_ = time(statbuf, 0, 0 ,1);
            user_UID_    = user_UID(statbuf);
            group_UID_   = group_UID(statbuf);
            user_NAME_   = user_NAME(user_UID);
            group_NAME_  = group_NAME(user_UID);
            type_        = findMediaType(magicNum, mediaTypes);
        }
        
    private:
        void isNull(bool setTo) {
            isNull_ = setTo;
        }
        
        int user_UID(struct stat &) {
            // https://ibm.co/2GwdIIR
            // Option to access via stat
            return static_cast<int>(statbuf.st_uid);
        }
        string user_NAME(int) {
            struct passwd *pwd;
            if((pwd = getpwuid(uid)) != NULL)
                return pwd->pw_name;
            else
                cerr << PROGNAME << ": " << uid << " not found in user database\n";
            return "uid not found";
        }
        int group_UID(struct stat&) {
            return statbuf.st_gid;
        }
        string group_NAME(int) {
            struct group *grp;
            if((grp = getgrgid(gid)) != NULL)
                return grp->gr_name;
            else
                cerr << PROGNAME << ": " << gid << " not found in group database\n";
            return "gid not found";
        }
        int permissions(struct stat &statbuf, string &output) {
            ostringstream os;
            if(S_ISDIR(statbuf.st_mode)) os <<  "d";
            else if(S_ISLNK(statbuf.st_mode)) os << "l";
            else if(S_ISREG(statbuf.st_mode)) os << "-";
            else {
                cerr << PROGNAME << ": " <<  path_ << " is an undefined file type, wtf?!\n";
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
        int size(struct stat &statbuf) {
            return statbuf.st_size;
        }
        string time(struct stat &statbuf, bool access = 0, bool mod = 1, bool status = 0) {
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
        vector< pair<string, string> > readMediaTypeFile(string dir) {
            vector< pair <string, string> > mediaTypes;
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
        string inttohex(int num) {
            // https://bit.ly/2InTEd9
            string d = "0123456789abcdef"; //
            string res;
            while(num > 0)
            {
                res = d[num % 16] + res;
                num /= 16;
            }
            if(res.length() == 1) res = "0" + res;
            return res;
        }

        string readMagicNumber(string dir) {
            ifstream file(dir);
            char chrctr;
            string magicNum;
            for ( int i = 0; i < 32; i++ ) {
                file.get(chrctr);
                //cout << "character: " << chrctr << "\n";
                if(chrctr < '!' || chrctr > '~') {
                    magicNum += "%" +  inttohex(chrctr);
                } else {
                    magicNum += chrctr;
                }
            }
            return magicNum;
        }

        string findMediaType(string magicNum, vector< pair<string, string> > MediaTypes, struct stat &statbuf) {
            if(S_ISDIR(statbuf.st_mode)) return "inode/directory";
            if(S_ISLNK(statbuf.st_mode)) return "inode/symlink";
            if(size(statbuf) == 0) return "inode/empty";
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
        
        bool isNull_;
        string access_time_;
        string mod_time_;
        string status_time_;
        string path_;
        string type_;
        int user_UID_;
        string permissions_;
        string group_NAME_;
        int group_UID_
        string user_NAME_;
        
}






