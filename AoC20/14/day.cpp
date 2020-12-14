#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <regex>
#include <numeric>
#include <assert.h>
#include <climits>

using namespace std;

typedef unsigned long long Int;

Int applyValue( const string & mask, Int v ) {
    const int n = mask.size();
    bitset<36> result( v );
    for ( int i = 1; i <= n; i++ ) {
        const char mv = mask[n - i];
        if ( mv != 'X') {
            result.set( i - 1, mv == '1' );
        }
    }
    return result.to_ullong();
}

int main() {

    const bool isFirstAnswer = false;

    ifstream f("input.txt");

    string line;
    // mask = 1000XX0X0X0X0011XX11110110X101101X01
    // mem[17353] = 91550
    const regex reMask("^mask = ([10X]+)$");
    const regex reMem("^mem\\[(\\d+)\\] = (\\d+)$");
    smatch what;

    string mask;
    map<Int,Int> mem;

    while (getline(f, line)) {
        const bool isMask = line[1] == 'a';
        if (isMask) {
            if( regex_match( line, what, reMask )) {
                mask = what[1];
            } else {
                cerr << "Unexpected line: " << line << endl;
            }
        } else {
            if( regex_match( line, what, reMem )) {
                const string a = what[1];
                const string v = what[2];
                const auto mv = applyValue( mask, stoull(v) );
                mem[stoull(a)] = mv;
            } else {
                cerr << "Unexpected line: " << line << endl;
            }
        }
    }
    Int sum = 0;
    for ( const auto [ a, v ] : mem ) {
        sum += v;
    }
    cout << "Answer 1: " << sum << endl;
    assert( sum == 13105044880745 );

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? 1 : 2 ) << endl;

    return 0;
}
