#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <climits>

using namespace std;

typedef pair<int,int> CountMin;
typedef map<int,CountMin> CountMinMap;

CountMin sum( const vector<int> & v, int b ) {
    auto result = 0;
    auto totalOn = 0;
    for ( auto i = 0; b; i++ ) {
        const bool on = b & 1;
        result += on ? v[i] : 0;
        totalOn += on;
        b >>= 1;
    }
    return { result, totalOn };
}

void init( const vector<int> & v, int b, CountMinMap & m ) {
    for ( ; b >= 0; b-- ) {
        const CountMin r = sum( v, b );
        const auto n = r.first;
        CountMin prev = m[n];
        if ( prev.first == 0 ) prev.second = r.second;
        m[n] = make_pair( prev.first + 1, min( prev.second, r.second ) );
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
    vector<int> containers;
    
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

    CountMinMap lowMap;
    vector<int> lowCo(containers.cbegin(), sp.first);
    init( lowCo, maxCounter(h), lowMap );

    CountMinMap highMap;
    const vector<int> highCo( sp.first, containers.end() );
    init( highCo, maxCounter( highCo.size() ), highMap );

    int minSum = INT_MAX;
    long long answer1 = 0;
    for ( auto cl : lowMap ) {
        const auto ch = highMap.find( TARGET - cl.first );
        if ( ch != highMap.end() ) {
            minSum = min(minSum, cl.second.second + ch->second.second);
            answer1 += ( cl.second.first * ch->second.first );
        }
    }
    cout << "Answer 1: " << answer1 << endl; // 4372 - right

    auto answer2 = 0;
    for ( auto c = maxCounter(containers.size()); c > 0; c-- ) {
        const auto p = sum(containers, c);
        answer2 += ( p.first == TARGET && p.second == minSum );
    }
    cout << "Answer 2: " << answer2 << endl; // 4 - right

    return 0;
}
