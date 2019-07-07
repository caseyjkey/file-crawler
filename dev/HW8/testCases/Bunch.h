#ifndef BUNCH_H
#define BUNCH_H

#include <string>
#include <sys/stat.h>       // For lstat
#include <set>
#include <pwd.h>
#include <grp.h>
#include <memory>           // For shared ptrs

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
    virtual ~Fing() = 0; // Dtor
    Fing(const Fing &); // Copy Ctor
    Fing(const std::string &, struct stat&); // Ctor
    
    
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
	
	struct stat statbuf_;
	
	static std::shared_ptr<const Fing> makeFing(const std::string &);
	virtual std::string type() const = 0;
      
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
    std::string time(bool = 0, bool = 1, bool = 0) const;
    bool        empty() const;
    
};

class Regular : public Fing {
  public:	
	~Regular() = default; // Dtor
    Regular(const Regular &); // Copy Ctor
    Regular(const std::string &); // Ctor
	using Fing::Fing;
	
	std::string type() const;
};

class Directory : public Fing {
  public:	
	~Directory() = default; // Dtor
    Directory(const Directory &); // Copy Ctor
    Directory(const std::string &); // Ctor
	using Fing::Fing;
  
	std::string type() const;
};

class Symlink : public Fing {
  public:	
    ~Symlink() = default; // Dtor
    Symlink(const Symlink &); // Copy Ctor
    Symlink(const std::string &); // Ctor
	using Fing::Fing;
  
	std::string type() const;
};

std::ostream &operator<<(std::ostream &, const Fing &);

struct fing_compare_ptr {
	bool operator() (std::shared_ptr<const Fing> x, std::shared_ptr<const Fing> y) const {
    //std::cout << "working with " << x.get()->path() << " and " << y.get()->path() << "\n";
		return (x.get()->statbuf_.st_dev + x.get()->statbuf_.st_ino) < (y.get()->statbuf_.st_dev + y.get()->statbuf_.st_ino);
	}
};

struct fing_compare_noptr {
	bool operator() (const Fing *x, const Fing *y) const {
		return (x->statbuf_.st_dev + x->statbuf_.st_ino) < (y->statbuf_.st_dev + y->statbuf_.st_ino);
	}
};

class Bunch {
  public:
    ~Bunch() = default;
    Bunch(const Bunch &);
    
    Bunch(const std::string &);
    
    Bunch &operator=(const Bunch &);
	
    Bunch operator+(const Bunch &) const;
    Bunch operator-(const Bunch &) const;
    
    Bunch &operator+=(const Bunch &);
    Bunch &operator-=(const Bunch &);
    
    bool operator==(const Bunch &) const;
    bool operator!=(const Bunch &) const;
    
    explicit operator bool() const;
	
    
    size_t     		                  size() const;      // number of entries 
    bool       		                  empty() const;
    const Fing *                      entry(size_t) const;
    std::string                       path() const;
    
    // S/O to S.O. https://bit.ly/2vEtBoU
	class iterator : public std::set<std::shared_ptr<const Fing>>::const_iterator {
	  public:
		iterator(typename std::set<std::shared_ptr<const Fing>>::const_iterator c) : std::set<std::shared_ptr<const Fing>>::const_iterator(c) { }
		const Fing * operator*() {
			return (std::set<std::shared_ptr<const Fing>>::const_iterator::operator *()).get();
		}
	};
    iterator begin() const {
        return iterator(entries.begin());
    }
    
    iterator end() const {
        return iterator(entries.end());
    }
    
  private:
    Bunch();                       // Throw an error for default constructor
    
    // ------------------ Bunch Attributes ----------------
    
    std::string        path_;
    
    // -------------------- Bunch Methods -----------------
    void updatePath();
    std::string traverse(const std::string &);
    
    std::set<std::shared_ptr<const Fing>, fing_compare_ptr> entries;
};



std::ostream &operator<<(std::ostream &, const Bunch &);

#endif