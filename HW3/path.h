#ifndef PATH_H
#define PATH_H

#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <pwd.h>
#include <grp.h>
using namespace std;

class Path {
    public:
        Path(string, string);
        static string PROGNAME;
        vector< pair<string, string> > mediaTypes;
        vector<pair<string, string>> readMediaTypeFile(string);
        int user_UID(string);
        int user_UID(struct stat &);
        string user_NAME(int);
        int group_UID(struct stat &);
        string group_NAME(int);
        int permissions(struct stat &, string &);
        int sizePath(struct stat &);
        void isNull(bool);
        string time(struct stat &, bool, bool, bool);
        string inttohex(int);
        string readMagicNumber(string);
        string findMediaType(string, vector< pair<string, string> >, struct stat &);
        void addEntry(string, string);
        
        // ------------------ Path Attributes ----------------
        string path_;
        string type_;
        int user_UID_;
        string permissions_;
        string group_NAME_;
        int group_UID_;
        string user_NAME_;
        string access_time_;
        string mod_time_;
        string status_time_;
        string magic_num_;
        vector<Path> entries;
        
        // ------------------ Helper Attributes ---------------
        bool isNull_;
        off_t size_;
        int numberOfEntries;
        
};

#endif


