#ifndef FIX_INCLUDED
#define FIX_INCLUDED

#include <iostream>
// This class is certainly inadequate.  It’s missing several ctors,
// has no assignment operator, etc.

template <typename T, int N>
class fix {
  public:
    typedef unsigned int size_type;
    typedef T *iterator;
    size_type size() const {
	return N;
    }
    T &operator[] (size_type index) {
	if(index >= size()) {
		std::cerr << "a.out: index out of bounds\n";	
		exit(1);
	}
	return data[index];
    }
    iterator begin() {
	return &data[0];
    }
    iterator end() {
	return &data[N];
    }
  private:
    T data[N];
};

#endif /* FIX_INCLUDED */
