#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include "hasher.h"
#include "hset_iter.h"

template <typename T, int N, typename H = std::hash<T>>
class hset {
  private:
    static constexpr unsigned table_size=N;
    std::vector< std::list<T> > table;

  public:
    typedef hset_iter<T> iterator;
    typedef T value_type;
    typedef size_t size_type;

    hset() : table(table_size) { }
    hset(const hset &) = default;
    hset &operator=(const hset &) = default;
    ~hset() = default;
    
    void insert(const T &datum) {
	unsigned hashval = H()(datum) % table.size();
	auto &lr = table[hashval];
	if (find(lr.begin(), lr.end(), datum) == lr.end())
	    lr.push_back(datum);
    }

    size_type size() const {
	size_type total = 0;
	for (const auto &row : table)
	    total += row.size();
	return total;
    }

    iterator begin() {
	return iterator(table, 0);
    }

    iterator end() {
	return iterator(table, size());
    }
    
    bool empty() const {
        std::all_of(table.cbegin(), table.cend(), [](const auto l) { return l.empty(); });
    }

    void dump() const {
	for (unsigned i=0; i<table.size(); i++) {
	    std::cout << "table[" << i << "]";
	    for (auto v : table[i]) 
		    std::cout << ' ' << v;
	    std::cout << '\n';
	}
    }
};
