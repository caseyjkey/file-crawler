#ifndef BUNCH_H
#define BUNCH_H

#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <pwd.h>
#include <grp.h>

class Bunch {
  public:
    ~Bunch();
    Bunch(const Bunch &);
    Bunch(const std::string); 
    Bunch(const std::string, const std::string);
    Bunch(const std::string, const std::string, const std::string);
    Bunch(const std::string, const std::string,  const std::string, bool);
    
    Bunch & operator=(const Bunch &);
    
    void        path(std::string); // replaces the path attribute of a Bunch, throw a std::string upon error including bad path
    void        all(bool);         // default arg is true
    size_t      size() const;      // number of entries 
    bool        empty() const;
    std::string entry(size_t) const;
    
  private:
    Bunch();                       // Throw an error for default constructor
    
    // ------------------ Bunch Attributes ----------------
    std::string       path_;
    
    // -------------------- Bunch Methods -----------------
    std::string traverse(std::string, bool);
    Bunch       &addEntry(std::string, std::string, std::string, bool);
    
    std::vector<Bunch> entries;
    std::vector<std::string> entryStrings;
      
    static std::string PROGNAME;
    std::string processFormatString(const Bunch&);
};

class Fing {
  public:
    ~Fing();
    Fing(const Fing &);
    Fing(const std::string); 
    Fing(const std::string, const std::string);
    Fing(const std::string, const std::string, const std::string);
    Fing(const std::string, const std::string, const std::string, bool);
    
    Fing & operator=(const Fing &);
    
    void        path(); 
    void        perms() const;
    void        uid() const;
    void        gid() const;
    size_t      size() const;      // number of entries 
    void        atime() const;
    void        mtime() const;
    void        ctime() const;
    
    
    std::string entry(size_t) ;
      
  private:
    
    // ------------------ Fing Attributes ----------------
    std::string path_;
    
    // ------------------ Helper Attributes ---------------
    off_t       fileSize_;
    struct stat statbuf;
    
    // --------------------- Methods ----------------------
      
    int         user_UID(const std::string);
    int         user_UID(const struct stat &);
    std::string user_NAME(int);
    int         group_UID(const struct stat &);
    std::string group_NAME(int);
    int         permissions(const struct stat &, std::string &);
    int         sizeOfPath(const struct stat &);
    std::string time(const struct stat &, bool, bool, bool);
    bool        empty() const;
    
    std::string inttohex(int);
};


std::ostream &operator<<(std::ostream &, const Fing &);

#endif


