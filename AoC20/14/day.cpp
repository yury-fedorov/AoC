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

const int Bits = 36;

Int applyValue( const string & mask, Int v ) {
    bitset<Bits> result( v );
    for ( int i = 1; i <= Bits; i++ ) {
        const char mv = mask[Bits - i];
        if ( mv != 'X') {
            result.set( i - 1, mv == '1' );
        }
    }
    return result.to_ullong();
}

vector<Int> getAddresses( const string & mask, const Int a ) {
    string na(mask);
    for ( int i = 0; i < Bits; i++ ) {
        const int j = Bits - i - 1;
        if ( mask[j] == '0' ) {
            const Int bit = ( 1l << i );
            const char value = ( ( a & bit ) ? '1' : '0' );
            na[j] = value;
        }
    }

    vector<int> xPos;
    for ( int i = 0; i < Bits; i++ ) {
        if ( na[i] == 'X' ) xPos.push_back(i);
    }
    
    vector<Int> list;
    const int xCount = xPos.size();
    if ( xCount == 0 ) {
        list.push_back( stoull( na, 0, 2 ) );
    } else {
        for ( int c = ( 1 << xCount ) - 1; c >= 0; c-- ) {
            string mc(na);
            int cc = c;
            for ( const int pos : xPos ) {
                mc[pos] = ( cc & 1 ) ? '1' : '0';
                cc >>= 1;
            }
            assert( mc.find( 'X' ) == string::npos ); // no X before parsing
            const auto nac = stoull( mc, 0, 2 );
            list.push_back( nac );
        }
    }
    return list;
}

void setMemory( Memory & mem, const string & mask, const Int a, const Int v ) {
    for ( const auto nac : getAddresses( mask, a) ) {
        mem[nac] = v;
    }
}

int main() {
    const bool isFirstAnswer = true;

    string mask;
    Memory mem;

    ifstream f("input.txt");

    string line;
    // mask = 1000XX0X0X0X0011XX11110110X101101X01
    // mem[17353] = 91550
    const regex reMask("^mask = ([10X]+)$");
    const regex reMem("^mem\\[(\\d+)\\] = (\\d+)$");
    smatch what;


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
    assert( sum == ( isFirstAnswer ? 13105044880745 : 3505392154485 ) );

    return 0;
}
