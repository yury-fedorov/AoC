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
typedef pair<int,int> BusPos;
typedef vector<BusPos> BusPosList;

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

BusPosList readBusPosList( const Input & buses, bool toPrint = false ) {
    BusPosList list;
    for ( int i = 0; i < buses.size(); i++ ) {
        const string & b = buses.at(i);
        if ( b == "x" ) continue;
        const int id = stoi(b);
        list.emplace_back( id, i );
    }
    // bus with highest numbers are at the beginning
    sort( list.begin(), list.end(), []( const BusPos & a, const BusPos & b ) { return b.first < a.first; } );
    if ( toPrint ) {
        for ( const auto [b,p] : list ) {
            cout << "Bus " << b << " wait: " << p << endl;
        }
    }
    return list;
}

Int ta( const BusPosList & bp ) {
    Int r = 1;
    for ( const auto [b,p] : bp ) {
        r *= b;
    }
    return r;
}
 
const auto tmin = 100'000'000'000'000;
const Int tmin1 = tmin / 100;

Int part2( const BusPosList & bp, Int t = 0 ) {
    const auto [ maxBus, maxBusShift ] = bp[0];
    for ( ; true; t += maxBus ) {
        bool isOk = true;
        for ( const auto [b,p] : bp ) {
            if ( b == maxBus ) continue;
            const long long tb = t + ( p - maxBusShift );
            if ( tb % b != 0 ) {
                isOk = false;
                break;
            }
        }
        if ( isOk ) {
            const auto answer2 = t - maxBusShift;
            const auto a = ta( bp );
            cout << "A2: " << answer2 << " " << a << " " << ( answer2 / double(a) ) << endl;
            return answer2;
        }
        if ( t % tmin1 == 0 ) {
            cout << "t = " << t << endl;
        }
    }
}

Int part2( const string & line, Int t = 0 ) {
    const auto && bp = readBusPosList( readBuses( line ), true );
    return part2( bp, t );
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
    if ( part2("17,x,13,19") != 3'417 ) assert(false);
    if ( part2("67,7,59,61" ) != 754'018 ) assert(false);
    if ( part2("67,x,7,59,61" ) != 779'210 ) assert(false);
    if ( part2("67,7,x,59,61" ) != 1'261'476 ) assert(false);
    if ( part2("1789,37,47,1889" ) != 1'202'161'486 ) assert(false);

    const BusPosList && busPos = readBusPosList( buses, true );
                    // 3'048'743'993'142'809
    const auto tmin = 100'000'000'000'000;
    const Int r = ta( busPos );
    const Int ts = r / 10;
    cout << r << " " << ts << endl;

    const auto a2 = part2(busPos, ts );
    cout << "Answer 2: " << a2 << endl;

    return 0;

/*


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
