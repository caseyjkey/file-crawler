#include <iostream>
#include <fstream>
#include <time.h>
#include <cassert>
#include <sstream>
using namespace std;

class Date {
  public:
    Date();
    // Default copy ctor is fine
    // Default assignment operator is fine
    Date(int year, int month, int day);

    int getday() const;
    int getmonth() const;
    int getyear() const;

    void setday(int);
    void setmonth(int);
    void setyear(int);

    Date &operator+=(int);
    int operator-(const Date &) const;

    Date operator+(int) const;
    const Date &operator++();	// prefix //why is this prefixed const?
    Date operator++(int);		// postfix

    Date operator-(int) const;
    Date &operator-=(int);
    const Date &operator--();	// prefix
    Date operator--(int);		// postfix

    bool operator<(const Date &) const;
    bool operator<=(const Date &) const;
    bool operator>(const Date &) const;
    bool operator>=(const Date &) const;
    bool operator==(const Date &) const;
    bool operator!=(const Date &) const;

  protected:
    static int ymd_to_daynum(int year, int month, int day);
    int daynum;				// 1 = January 1, year 1
};

Date operator+(int, const Date &);
ostream &operator<<(ostream &out, const Date &);	// YYYY-MM-DD
istream &operator>>(istream &in, Date &);		// YYYY-MM-DD

static bool leap(int year) {
    assert(1 <= year && year <= 9999);
    if (year % 4 != 0)
	return false;
    if (year % 100 != 0)
	return true;
    if (year % 400 != 0)
        return false;
    /*
    if (year % 400 == 0)			// Divisible by 400?
	return true;				// It’s a leap year.
    if (year % 100 == 0)			// Divisible by 100?
	return false;				// Not a leap year.
    if (year % 4 == 0)				// Divisible by 4?
	return true;				// It’s a leap year.
     */
     return false;				// Boring old ordinary year.
}

static int days_per_year(int year) {
    assert(1 <= year && year <= 9999);
    return leap(year) ? 366 : 365;
}

static int days_per_month(int year, int month) {
    assert(1 <= year && year <= 9999);
    assert(1 <= month && month <= 12);
    const int days[] = {00,31,28,31,30,31,30,31,31,30,31,30,31};
    if (month==2 && leap(year))
	return 29;
    return days[month];
}

int Date::ymd_to_daynum(int year, int month, int day) {
    assert(1 <= year && year <= 9999);
    assert(1 <= month && month <= 12);
    assert(1 <= day && day <= 31);

    // Start at Jan 1, 0001, and increment until we get to what we want
    int daynum=1;

    for (int y=1; y < year; y++)
	daynum += days_per_year(y);

    for (int m=1; m < month; m++)
	daynum += days_per_month(year, m);

    // We're now poised at the first day of that month
    daynum += day-1;

    assert(1 <= daynum && daynum <= 10000 * 366);
    return daynum;
}

static void daynum_to_ymd(int daynum, int &year, int &month, int &day) {
    assert(1 <= daynum && daynum <= 10000 * 366);

    int y=1, m=1, d=1;

    // Jump forward by years, at first.  This reduces the iterations,
    // for modern dates, from ~733k to ~2181.
    while (daynum >= 366)
	daynum -= days_per_year(y++);

    for (; daynum>1; daynum--) {
	// Increment y-m-d
	d++;
	if (d > days_per_month(y, m)) {
	    m++;
	    d=1;
	    if (m > 12) {
		y++;
		m=1;
	    }
	}
    }

    year = y;
    month = m;
    day = d;

    assert(1 <= year && year <= 9999);
    assert(1 <= month && month <= 12);
    assert(1 <= day && day <= 31);
}

Date::Date(int year, int month, int day)
: daynum(ymd_to_daynum(year, month, day)) {
    assert(1 <= year && year <= 9999);
    assert(1 <= month && month <= 12);
    assert(1 <= day && day <= 31);
    assert(1 <= daynum && daynum <= 10000 * 366);
}

Date::Date() {
    const time_t now = time(nullptr);	    // Seconds since 1970 started
    struct tm *tm = localtime(&now);	    // year/month/day/hour/min/sec
    daynum = ymd_to_daynum(tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday);
    // Yes, 1900.  That’s not a Y2K bug--that’s how localtime is defined.
}

int Date::getday() const {
    int year, month, day;
    daynum_to_ymd(daynum, year, month, day);
    return day;
}

int Date::getmonth() const {
    int year, month, day;
    daynum_to_ymd(daynum, year, month, day);
    return month;
}

int Date::getyear() const {
    int year, month, day;
    daynum_to_ymd(daynum, year, month, day);
    return year;
}

void Date::setday(int new_day) {
    int year, month, day;
    daynum_to_ymd(daynum, year, month, day);
    daynum = ymd_to_daynum(year, month, new_day);
}

void Date::setmonth(int new_month) {
    int year, month, day;
    daynum_to_ymd(daynum, year, month, day);
    daynum = ymd_to_daynum(year, new_month, day);
}

void Date::setyear(int new_year) {
    int year, month, day;
    daynum_to_ymd(daynum, year, month, day);
    daynum = ymd_to_daynum(new_year, month, day);
}

// This one stands alone

int Date::operator-(const Date &rhs) const {
    return daynum - rhs.daynum;
}

// This does the real work of all numeric operations.
// All other operators (+, -, ++, -=, etc) should invoke operator+=.

Date &Date::operator+=(int value) {
    daynum += value;
    return *this;
}

Date Date::operator+(int value) const {
    Date result = *this;
    return result += value;
}

const Date &Date::operator++() {			// prefix
    *this += 1;
    return *this;
}

Date Date::operator++(int /* dummy */) {		// postfix
    const auto before = *this;
    *this += 1;
    return before;
}

Date Date::operator-(int value) const {
    return *this + -value;
}

Date &Date::operator-=(int value) {
    return *this += -value;
}

const Date &Date::operator--() {			// prefix
    *this -= 1;
    return *this;
}

Date Date::operator--(int /* dummy */) {		// postfix
    Date before = *this;
    *this -= 1;
    return before;
}


bool Date::operator<(const Date &rhs) const {
    return daynum < rhs.daynum;
}

bool Date::operator<=(const Date &rhs) const {
    return daynum <= rhs.daynum;
}

bool Date::operator>(const Date &rhs) const {
    return daynum > rhs.daynum;
}

bool Date::operator>=(const Date &rhs) const {
    return daynum >= rhs.daynum;
}

bool Date::operator==(const Date &rhs) const {
    return daynum == rhs.daynum;
}

bool Date::operator!=(const Date &rhs) const {
    return daynum != rhs.daynum;
}

Date operator+(int value, const Date &rhs) {
    return rhs + value;
}

ostream &operator<<(ostream &out, const Date &d) {
    return out << d.getyear() << '-' << d.getmonth() << '-' << d.getday();
}

#include <iomanip>

istream &operator>>(istream &in, Date &d) {
    int year, month, day;
    char c1, c2;
    if (in >> year  >> c1 && c1=='-'
     && in >> month >> c2 && c2=='-'
     && in >> day)
	d = Date(year, month, day);
    else
	in.setstate(ios::failbit);	// Input failed, so spoil the stream
    return in;
}


int main() {
    // Read & display some dates
    ifstream data("data");
    Date d;
    // Read each line, try to convert it to a date, complain if it fails.
    for (string s; getline(data, s); ) {
	istringstream iss(s);	// Initialize stream with string
	if (iss >> d)		// Try to convert to a date
	    cout << d << '\n';
	else
	    cerr << "Bad date \"" << s << "\"\n";
    }

    const Date a(2009,3,4);
    cout << a << '\n';		// Wed Mar 4 2009

    const Date b(a);
    cout << b << '\n';		// Wed Mar 4 2009

    Date c; c=a;
    cout << c << '\n';		// Wed Mar 4 2009

    c = c+1;
    cout << c << '\n';		// Thu Mar 5 2009
    c += 9;
    cout << c << '\n';		// Sat Mar 14 2009
    d = ++c;
    cout << c << '\n';		// Sun Mar 15 2009
    cout << d << '\n';		// Sun Mar 15 2009

    d = c++;
    cout << c << '\n';		// Mon Mar 16 2009
    cout << d << '\n';		// Sun Mar 15 2009

    Date e = d-1;
    cout << e << '\n';		// Sat Mar 14 2009
    d = e--;
    cout << d << '\n';		// Sat Mar 14 2009
    cout << e << '\n';		// Fri Mar 13 2009
    d = --e;
    cout << d << '\n';		// Thu Mar 12 2009
    cout << e << '\n';		// Thu Mar 12 2009
    e -= 5;
    cout << e << '\n';		// Sat Mar 7 2009
    e -= -2;
    cout << e << '\n';		// Mon Mar 9 2009
    cout << d-e << '\n';	// 3
    cout << e-d << '\n';	// -3

    const Date today(2019, 3, 15);
    Date yesterday;
    yesterday.setmonth(3);
    yesterday.setday(14);
    yesterday.setyear(2019);
    cout << yesterday << today << '\n';

    // a==b, a<c
    cout << ((a==b) ? "== good\n" : "== bad\n");
    cout << ((a==c) ? "== bad\n"  : "== good\n");
    cout << ((a!=c) ? "!= good\n" : "!= bad\n");
    cout << ((a!=b) ? "!= bad\n"  : "!= good\n");
    cout << ((a<c)  ? "<  good\n" : "<  bad\n");
    cout << ((a<b)  ? "<  bad\n"  : "<  good\n");
    cout << ((c>a)  ? ">  good\n" : ">  bad\n");
    cout << ((b>a)  ? ">  bad\n"  : ">  good\n");
    cout << ((a<=b) ? "<= good\n" : "<= bad\n");
    cout << ((a<=c) ? "<= good\n" : "<= bad\n");
    cout << ((c<=a) ? "<= bad\n"  : "<= good\n");
    cout << ((a>=b) ? ">= good\n" : ">= bad\n");
    cout << ((c>=a) ? ">= good\n" : ">= bad\n");
    cout << ((a>=c) ? ">= bad\n"  : ">= good\n");

    cout << ((today == (1 + yesterday)) ? "setters good\n" : "setters bad\n");

    return 0;
}
