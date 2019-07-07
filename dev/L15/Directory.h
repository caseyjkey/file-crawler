#ifndef DIRECTORY_H_INCLUDED
#define DIRECTORY_H_INCLUDED

#include <dirent.h>
#include <string>
#include <regex>

// Normally, I’d have DirectoryIterator be a nested class, Directory::iterator,
// to avoid namespace pollution.  However, this is easier to read.

class DirectoryIterator {
  public:
    DirectoryIterator() = default;
    DirectoryIterator(DIR *, const std::regex &re);
    DirectoryIterator &operator++();
    std::string operator*() const;
    bool operator!=(const DirectoryIterator &) const;
  private:
    DIR *dp = nullptr;
    bool wanted() const;
    std::string name;
    std::regex re;
};

class Directory {
  public:
    typedef DirectoryIterator iterator;
    Directory() = delete;
    Directory(const char dirname[], const std::string &pattern = "*");
    ~Directory();
    iterator begin() const;
    iterator end() const;
  private:
    DIR *dp;				// null if open failed
    std::regex re;
};

#endif /* DIRECTORY_H_INCLUDED */
