#include <iostream>
#include <deque>
#include <vector>
#include <fstream>
#include <sstream>
#include <string_view>
#include <catch2/catch.hpp>

using namespace std;

template<class T>
char pop(deque<T> & c) {
    const T ch = c[0];
    c.pop_front();
    return ch;
}

// question 1
string toMemory( string_view line ) {
    const auto l = line.length();
    const auto d = line.data();
    deque<char> c( d + 1, d + l - 1 ); // remove first and last
    vector<char> v;
    while (!c.empty()) {
        const auto next = pop(c);
        if ( next != '\\' ) {
            v.push_back(next);
        } else {
            const auto next1 = pop(c);
            if ( next1 != 'x' ) {
                v.push_back(next1);
            } else {
                const auto next2 = pop(c);
                const auto next3 = pop(c);
                v.push_back('?'); // we care only about size
            }
        }
    }
    return string(v.begin(), v.end());
}

// question 2
string fromMemory(string_view line) {
    const auto l = line.length();
    const auto d = line.data();
    deque<char> c( d, d + l );
    vector<char> v;
    v.push_back('\"');
    while (!c.empty()) {
        const auto next = pop(c);
        if ( next == '\\' || next == '"' ) {
            v.push_back( '\\' );
        }
        v.push_back(next);
    }
    v.push_back('\"');
    return string(v.begin(), v.end());
}

auto day08( const bool isFirstAnswer ) {
    ifstream f("08/input.txt");

    int codeLength = 0;
    int textLength = 0;

    string line;
    while (getline(f, line)) {
        const int l = line.length();
        codeLength += l;
        const string s = isFirstAnswer ? toMemory(line) : fromMemory(line);
        const int l1 = s.length();
        textLength += l1;
    }
    return abs(codeLength - textLength);
}

TEST_CASE( "Day08", "[08]" ) {
    REQUIRE(1333 == day08(true));
    REQUIRE(2046  == day08(false));
}
