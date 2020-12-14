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
#include <cmath>

using namespace std;

typedef unsigned long long Int;
typedef map<Int,Int> Memory;

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

void setMemory( Memory & mem, const string & mask, const Int a, const Int v ) {
    const int n = mask.size();
    string rm(mask);
    reverse(rm.begin(), rm.end());
    string na;
    for ( int i = 0; i < n; i++ ) {
        const char mv = rm[i];
        switch ( mv ) {
            case '0': na = ( ( a & ( 1 << i ) ) ? "1" : "0" ) + na; break;
            case '1': na = "1" + na; break;
            case 'X': na = "X" + na; break;
        }
    }
    /*
    cout << mask << endl;
    cout << bitset<36>(a).to_string() << endl;
    cout << na << endl;
    cout << endl;
    */

    vector<int> xPos;
    for ( int i = 0; i < n; i++ ) {
        if ( na[i] == 'X' ) xPos.push_back(i);
    }
    const int xCount = xPos.size();
    if ( xCount == 0 ) {
        mem[ stoull( na, 0, 2 ) ] = v;
    } else {
        int count = 0;
        for ( int c = ( 1 << xCount ) - 1; c >= 0; c-- ) {
            string mc(na);
            int cc = c;
            for ( const int pos : xPos ) {
                mc[pos] = ( cc & 1 ) ? '1' : '0';
                cc >>= 1;
            }
            assert( mc.find( 'X' ) == string::npos ); // no X before parsing
            cout << na << endl;
            cout << mc << endl << endl;
            const auto nac = stoull( mc, 0, 2 );
            mem[nac] = v;
            count++;
        }
        assert( count == (int)pow( 2.0, xCount ) );
    }
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
    Memory mem;

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
                const auto a = stoull( what[1] );
                const auto v = stoull( what[2] );
                if ( isFirstAnswer ) {
                    mem[a] = applyValue( mask, v );
                } else {
                    setMemory( mem, mask, a, v );
                }

            } else {
                cerr << "Unexpected line: " << line << endl;
            }
        }
    }
    // XXX accomulate ?
    Int sum = 0;
    for ( const auto [ a, v ] : mem ) {
        sum += v;
    }

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << sum << endl;
    assert( sum == ( isFirstAnswer ? 13105044880745 : 0 ) );
    // 3530622174725 - answer2 - That's not the right answer; your answer is too high.

    return 0;
}
