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

typedef long long Int;
typedef vector<string> Input;

Input readBuses( const string & line ) {
    Input buses;
    regex re("([x0-9]+)");
    smatch res;
    string::const_iterator searchStart( line.cbegin() );
    while ( regex_search( searchStart, line.cend(), res, re ) )
    {
        const string busId = res[1];
        buses.push_back( busId );
        searchStart = res.suffix().first;
    }
    return buses;
}

auto aPlus1( Int n, Int m, Int b = 1 ) {
    return div( b * (m + 1), n );
}

int main() {
    ifstream f("input.txt");
    string line;
    f >> line;
    const int t0 = stoi(line);
    f >> line;
    const Input && buses = readBuses(line);
    
    int bestBus = 0;
    int bestTime = INT_MAX;
    for ( const string & bid : buses ) {
        if (bid == "x") continue;
        const int b = stoi(bid);
        const int wait = b - (t0 % b);
        if (bestTime > wait) {
            bestTime = wait;
            bestBus = b;
        }
    }
    const auto answer1 = bestBus * bestTime;
    cout << bestBus << ',' << bestTime << '=' << answer1 << endl; 
    assert( answer1 == 296 ); // 296 - answer 1

    // Part 2
    typedef pair<int,int> BusPos;
    vector<BusPos> busPos;
    for ( int i = 0; i < buses.size(); i++ ) {
        const string & b = buses[i];
        if ( b == "x" ) continue;
        const int id = stoi(b);
        busPos.emplace_back( id, i );
    }
    // bus with highest numbers are at the beginning
    sort( busPos.begin(), busPos.end(), []( const BusPos & a, const BusPos & b ) { return b.first < a.first; } ); 
    for ( const auto [b,p] : busPos ) {
        cout << "Bus " << b << " wait: " << p << endl;
    }

    const auto tmin = 100'000'000'000'000;
    return 0;

/*
    for ( Int t = 0; true; t += maxBus ) {
        // this is ok
        bool isOk = true;
        for ( const auto [b,p] : busPos ) {
            if ( b == maxBus ) continue;
            const long long tb = t + ( p - maxBusShift );
            if ( tb % b != 0 ) {
                isOk = false;
                break;
            }
        }
        if ( isOk ) {
            cout << "A2: " << t << endl;
            break;
        }
    }

    const Int b0 = div( t0, maxBus ).quot + 1;
    Int t = b0 * maxBus;
    for ( auto b = b0; true; b++, t+= maxBus ) {
        for ( const auto [bus,p] : busPos ) {
            if ( bus == maxBus ) continue;
            const long long tb = t + ( p - maxBusShift );
            if ( tb % bus != 0 ) {
                isOk = false;
                break;
            }
        }
    }
*/
    return 0;
}
