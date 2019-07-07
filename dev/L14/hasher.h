#include <string>

// Some wretched hash functions
//
// They should return unsigned values, as equally-distributed as
// possible over the range of unsigned values.

class Hasher {
  public:
    unsigned operator()(int n) const {
	return n;			// That was easy!
    }

    unsigned operator()(double d) const {
        return std::hash<double>()(d);
    }

    // A terrible hash function--ASCII strings less than a hundred
    // bytes will only add up to less than 12800.  Also, they will
    // strongly cluster at 100*(average value for a printable character)
    // That’s a poor distribution.

    unsigned operator()(const std::string &s) const {
	unsigned sum = 42;
	for (char c : s)
	    sum += c;
	return sum;
    }
};

