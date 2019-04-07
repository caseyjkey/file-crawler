#ifndef BUNCH_H
#define BUNCH_H

#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <pwd.h>
#include <grp.h>

// Credit to Jack "Mr. C" Applin
// https://bit.ly/2UkmGQt

template <typename T, typename U>
class Dual {
    const T t;
    const U u;
  public:
    Dual(const T &tt, const U &uu) : t(tt), u(uu) { }
    operator T() const { return t; }
    operator U() const { return u; }
};

class Fing {
  static std::string PROGNAME;  
    
  public:
    ~Fing();
    Fing(const Fing &);
    Fing(const std::string &, bool);
    
    Fing & operator=(const Fing &);
    
    std::string                     path() const; 
    Dual<std::string, int>          perms() const;
    Dual<std::string, uid_t>        uid() const;
    Dual<std::string, gid_t>        gid() const;
    size_t                          size() const;      // number of entries 
    Dual<std::string, time_t>       atime() const;
    Dual<std::string, time_t>       mtime() const;
    Dual<std::string, time_t>       ctime() const;
      
  private:
    Fing();
      
      
    // ------------------ Fing Attributes ----------------
    std::string path_;
    bool        all_;
    
    // ------------------ Helper Attributes ---------------
    off_t       fileSize_;
    struct stat statbuf;
    
    // --------------------- Methods ----------------------
      
    int         user_UID(const std::string) const;
    int         user_UID(const struct stat &) const;
    std::string user_NAME(uid_t) const;
    int         group_UID(const struct stat &) const;
    std::string group_NAME(gid_t) const;
    std::string permissions() const;
    std::string time(bool, bool, bool) const;
    bool        empty() const;
    
};

std::ostream &operator<<(std::ostream &, const Fing &);

class Bunch {
  public:
    ~Bunch();
    Bunch(const Bunch &);
    
    Bunch(const std::string);
    Bunch(const std::string, bool);
    
    Bunch & operator=(const Bunch &);
    
    std::string        path(const std::string &); // replaces the path attribute of a Bunch, throw a std::string upon error including bad path
    void        	   all(bool);         // default arg is true
    size_t     		   size() const;      // number of entries 
    bool       		   empty() const;
    Fing		       entry(size_t) const;
    
  private:
    Bunch();                       // Throw an error for default constructor
    
    // ------------------ Bunch Attributes ----------------
    std::string       path_;
    bool              all_;
    
    // -------------------- Bunch Methods -----------------
    std::string traverse(const std::string &);
    
    std::vector<Fing> entries;
    std::vector<std::string> entryStrings;
      
    static std::string PROGNAME;
    std::string processFormatString(const Bunch&);
};

#endif


