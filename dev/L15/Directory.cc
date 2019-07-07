#include "Directory.h"
#include <iostream>
#include <regex>

using namespace std;

// The directory iterator pre-fetches the name.  That is, readdir()
// is called when you first create the iterator, and upon ++.
//
// An end() iterator is designated by a null pointer for its DIR *.
// When readdir() fails, we set our pointer to null.

DirectoryIterator::DirectoryIterator(DIR *dp_arg, const regex &re) : dp(dp_arg), re(re) {
    ++*this;				// get that first entry
}

DirectoryIterator &DirectoryIterator::operator++() {	// pre-increment
    while (auto p = readdir(dp)) {	// Read a directory entry.
	name = p->d_name;		// C-string ⇒ C++ string
	if(wanted())
            return *this;		// Everything I'd like!
    }
    dp = nullptr;			// readdir failed; don’t try again.
    return *this;
}

std::string DirectoryIterator::operator*() const {
    return name;			// Fetched in a previous ++.
}

bool DirectoryIterator::operator!=(const DirectoryIterator &rhs) const {
    return dp != rhs.dp;
}

bool DirectoryIterator::wanted() const {
    if (name == ".")
        return false;
    if ( name == "..")
        return false;
    return regex_search(name, re);
}

//////////////// Directory methods

Directory::Directory(const char dirname[], const string &pattern) : re(pattern)  {
	auto p = opendir(dirname);
	if (!p) {
		cerr << " Bad directory" << endl;
		exit(1);
	}
	dp = p;
}

Directory::~Directory() {
    if (dp)				// Only if opendir() succeeded:
	closedir(dp);			//   should we close it.
}

Directory::iterator Directory::begin() const {
    // Should I open it here, instead?  Later seems better.
    return iterator(dp, re);
}

Directory::iterator Directory::end() const {
    return iterator();
}
