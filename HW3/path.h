#ifndef PATH_H
#define PATH_H
#include string
#include <sys/stat.h>
#include <vector.h>
#include <pwd.h>
#include <grp.h>

class Path (
    public:
        Path(string path_);
        int user_UID(std::string);
        int user_UID(struct stat &);
        std::string path();
        std::string user_NAME(int);
        int group_UID(struct stat);
        std::string group_NAME(int gid);
        int permissions(struct stat&, std::string &);
        int size(struct stat);
        std::string time(struct stat, bool, bool, bool);
        vector<std::pair<std::string, std::string>> readMediaTypeFile(std::string);
        std::string inttohex(int);
        std::string readMagicNumber(std::string);
        std::string findMediaType(std::string, std::vector< pair<string, string> >);
    private:
        std::string path_;
        std::string type_;
        int user_UID_;
        std::string permissions_;
        std::string group_NAME_;
        int group_UID_
        std::string user_NAME_;
        int access_time_;
        int mod_time_;
        int status_time_;
};

#endif PATH_H


