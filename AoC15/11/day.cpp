#include <iostream>
#include <sstream>
#include <string_view>
#include <catch2/catch.hpp>

using namespace std;

bool containsIncreasing3Letters(string_view pwd) {
    const int n = pwd.length();
    for ( auto i = 2; i < n; i++ ) {
        if ( ( pwd[i] - pwd[i-1] ) == 1 && ( pwd[i-1] - pwd[i-2] ) == 1 )
          return true;
    }
    return false;
}

bool noIol(string_view pwd) {
    return !any_of( begin(pwd), end(pwd), [] (char c) { return c == 'i' || c == 'o' || c == 'l'; } );
}

bool contains2Pairs(string_view pwd) {
    auto pairs = 0;
    char last = '?';
    auto count = 0;
    string s(pwd);
    s +="$"; // to simply algorithm
    for ( auto ch : s ) {
        if ( ch == last ) count++;
        else { 
            pairs += count >= 2 ? 1 : 0;
            last = ch;
            count = 1;
        }
    }
    return pairs >= 2;
}

bool isValidPassword(string_view pwd) {
    return containsIncreasing3Letters(pwd) && noIol(pwd) && contains2Pairs(pwd);
}

string next(string_view pwd) {
    const int n = pwd.length();
    string result(pwd);
    auto i = n - 1;
    while ( i >= 0 ) {
        if ( result[i] != 'z' ) {
            result[i]++;
            break;
        } else {
            result[i--] = 'a';
        }
    }
    return result;
}

TEST_CASE( "Day11", "[11]" ) {
    string input = "hepxcrrq";
    for ( ; !isValidPassword(input); input = next(input) ) {}
    REQUIRE( "hepxxyzz" == input );

    do {
        input = next(input);
    } while (!isValidPassword(input));
    REQUIRE( "heqaabcc" == input );
}
