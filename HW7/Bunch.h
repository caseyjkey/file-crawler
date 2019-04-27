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
    
  public:
    ~Fing(); // Dtor
    Fing(const Fing &); // Copy Ctor
    Fing(const std::string &); // Ctor
    
    Fing & operator=(const Fing &);
    bool operator==(const Fing &) const;
    
    std::string                     path() const; 
    Dual<std::string, int>          perms() const;
    Dual<std::string, uid_t>        uid() const;
    Dual<std::string, gid_t>        gid() const;
    size_t                          size() const;      // number of entries 
    Dual<std::string, time_t>       atime() const;
    Dual<std::string, time_t>       mtime() const;
    Dual<std::string, time_t>       ctime() const;
      
  private:
    Fing(); // No-arg Ctor is an Error
      
      
    // ------------------ Fing Attributes ----------------
    std::string path_;
    
    // ------------------ Helper Attributes ---------------
    off_t       fileSize_;
    
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
    
    Bunch(const std::string &);
    
    Bunch &operator=(const Bunch &);
	
	Bunch operator+(const Bunch &) const;
	Bunch operator-(const Bunch &) const;
	
	Bunch operator+=(const Bunch &);
	Bunch operator-=(const Bunch &);
	
	bool operator==(const Bunch &) const;
	bool operator!=(const Bunch &) const;
    
    explicit operator bool() const;
	
    
    size_t     		   size() const;      // number of entries 
    bool       		   empty() const;
    const Fing *       entry(size_t) const;
    bool               addEntry(const Fing &); // we could use smart pointers to store references to Fings in entries
    std::string        path() const;
    
    typedef std::vector<const Fing *>::iterator iterator;
    
    iterator begin() const {
        return entries.begin();
    }
    
    iterator end() const {
        return entries.end();
    }
    
  private:
    Bunch();                       // Throw an error for default constructor
    
    // ------------------ Bunch Attributes ----------------
    
    std::string        path_;
    
    // -------------------- Bunch Methods -----------------
    void updatePath();
    std::string traverse(const std::string &);
    
    std::vector<const Fing *> entries;
};

std::ostream &operator<<(std::ostream &, const Bunch &);

#endif


