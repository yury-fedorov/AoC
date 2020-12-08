#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

int sum( const vector<int> & v, int b ) {
    auto result = 0;
    for ( auto i = 0; b; b >>= 1, i++ ) {
        result += ( b & 1 ) ? v[i] : 0;
    }
    return result;
}

void init( const vector<int> & v, int b, map<int,int> & m ) {
    for ( ; b > 0; b-- ) {
        const auto n = sum( v, b );
        if ( n == 120 ) cout << b << endl;
        m[n] += 1;
    }
}

int maxCounter( int n ) {
    return ( 1 << n ) - 1; // sets all valid bits to 1
}

auto split( const vector<int> & containers, const int target ) {
    auto low = 0;
    auto it = containers.cbegin();
    for ( ; it != containers.cend(); it++ ) {
        const auto next = low + *it;
        if ( next >= target ) { 
            break;
        }
        low = next;
    }
    return make_pair( it, low );
}

int main() {
    const bool isFirstAnswer = true;

    vector<int> containers;
    /* {20, 15, 10, 5, 5};
    const int TARGET = 25; */
    
    ifstream f("input.txt");
    string line;
    while (getline(f, line)) {
        containers.push_back( stoi(line) );
    }
    const int TARGET = 150;
    
    sort( containers.begin(), containers.end() );

    const auto sp = split(containers, TARGET);
    const auto h = sp.first - containers.cbegin();
    const auto firstHigh = *sp.first;
    // cout << sp.second << " " << h << " " << firstHigh << endl;

    map<int, int> lowMap;
    vector<int> lowCo(containers.cbegin(), sp.first);
    init( lowCo, maxCounter(h), lowMap );

    map<int, int> highMap;
    const vector<int> highCo( sp.first, containers.end() );
    init( highCo, maxCounter( highCo.size() ), highMap );

    long long counter = 0;
    for ( auto cl : lowMap ) {
        const auto ch = highMap.find( TARGET - cl.first );
        if ( ch != highMap.end() ) {
            cout << cl.first << " + " << ch->first << " -> " << cl.second << " * " << ch->second << " = " << (cl.second * ch->second) << endl;
            counter += ( cl.second * ch->second );
        }
    }
    // 6732 - too high
    // 4369 - is not the right
    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? counter : 2 ) << endl;

    return 0;
}
