#ifndef TEST_H
#define TEST_H

#include <cstddef>
#include <string>
#include <set>
#include <iostream>
#include <string>
#include <set>
#include <sstream>

#define MAX_STUDENT_LINES_LENGTH 500

// Every test case implements the following function:
int run_test_case(std::string &info);

void report(const std::string &info);

std::string& operator+=(std::string &s, const std::multiset<std::string> &sorted_lines);
std::string truncated_if_too_long(const std::string &s);

#define CHECK_LINES(ACTUAL, EXPECTED) \
ostringstream info_stream; \
if (EXPECTED != ACTUAL) { \
    info_stream << "Wrong answer:\n" \
                << "Expected in " #ACTUAL ":\n" \
                << EXPECTED \
                << "Yours:\n" \
                << truncated_if_too_long(ACTUAL) \
                << "(Yours ends here)" \
                << endl; \
    info = info_stream.str(); \
    return 1; \
}

#define EXPECT_THROWN_STRING_TO_CONTAIN(KEYWORD) \
catch (string s) { \
    if (s.find(KEYWORD) == string::npos) { \
        info = "The error string doesn't contain the offending argument: " KEYWORD; \
        return 1; \
    } \
    return 0; \
} \
info = "No thrown string for the offending argument: " KEYWORD; \
return 1;

#define EXPECT(EXPR) \
if (!(EXPR)) { \
    info = "Wrong answer: assertion '" #EXPR "' failed"; \
    return 1; \
}

#endif //TEST_H
