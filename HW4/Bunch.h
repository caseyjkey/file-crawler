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
		Bunch();
		Bunch(const std::string); 
		Bunch(const std::string, const std::string);
		Bunch(const std::string, const std::string, const std::string);
        Bunch(const std::string, const std::string,  const std::string, bool);
		
        void              path(std::string); // replaces the path attribute of a Bunch, throw a std::string upon error including bad path
		void              magic(std::string); // Same rules as above regarding errors
		void              format(std::string);  // default arg is %p %U %G %s %n
		void              all(std::string); // default arg is true
		const size_t      size(); // number of entries 
		const bool        empty();
		const std::string entry(size_t);
		
		
        std:vector< pair<std::string, std::string> > mediaTypes;
        std:vector<pair<std::string, std::string>> readMediaTypeFile(std::string);
        int         user_UID(std::string);
        int         user_UID(struct stat &);
        std::string user_NAME(int);
        int         group_UID(struct stat &);
        std::string group_NAME(int);
        int         permissions(struct stat &, std::string &);
        int         sizePath(struct stat &);
        void        isNull(bool);
        std::string time(struct stat &, bool, bool, bool);
        std::string inttohex(int);
        std::string readMagicNumber(std::string);
        std::string findMediaType(std::string, std:vector< pair<std::string, std::string> >, struct stat &);
		Bunch       traverse(Bunch, std::string, std::string, bool);
        Bunch       &addEntry(std::string, std::string, std::string, bool);
        
        // ------------------ Bunch Attributes ----------------
        std::string       path_;
        std::string       type_;
        std::string       permissions_;
        int               user_UID_;
        int               group_UID_;
        std::string       group_NAME_;
        std::string       user_NAME_;
        std::string       access_time_;
        std::string       mod_time_;
        std::string       status_time_;
        std::string       magic_num_;
        std::string       format_;
        bool              all_;
        std:vector<Bunch> entries;
        
        // ------------------ Helper Attributes ---------------
        bool isNull_;
        off_t fileSize_;
        int numberOfEntries;
        
	private:
		static std::string PROGNAME;
};

#endif


