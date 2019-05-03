#ifndef FIX3_INCLUDED
#define FIX3_INCLUDED
#include <iostream>

// This class is certainly inadequate.  It’s missing several ctors,
// has no assignment operator, etc.

template <typename T, int N>
class fix {
  public:
    typedef unsigned int size_type;
    class iterator {
      public:
	iterator() : base(nullptr), index(0) {}
	iterator(fix *b, size_type i) : base(b), index(i) { }
        iterator &operator++() {
	    if(base->size() <= index) {
		std::cerr << "fix3.h: index out of range";
	 	exit(1);
	    }
	    index++;
	    return *this;
	}
	T &operator*() {
	    return (*base)[index];
	}
      private:
	fix *base;
	size_type index;
    };

    size_type size() const {
	return N;
    }
    T &operator[] (size_type index) {
	return data[index];
    }
    iterator begin() {
	return iterator(this, 0);
    }
    iterator end() {
	return iterator(this, N);
    }
  private:
    T data[N];
};

#endif /* FIX3_INCLUDED */
