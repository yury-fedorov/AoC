#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
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

pair<int,Int> part2check2( const BusPosList & bp, Int t ) {
    int index = -1;
    Int step = 1;
    for ( const auto [b,p] : bp ) {
        const Int tb = t + p;
        if ( tb % b != 0 ) {
            break;
        }
        index++;
        step *= b;
    }
    return make_pair( index, step );;
}

Int part2( const BusPosList & bp, Int t = 0 ) {
    Int step = 1;
    const int n = bp.size();
    const int targetIndex = n - 1;
    for ( ; true; t += step ) {
        // if ( part2check( bp, t ) ) {
        const auto [i, s] = part2check2(bp, t );
        if ( i >= targetIndex ) {
            return t;
        }
        if ( step < s ) {
            cout << "Current step: " << step << " new step: " << s << endl;
            step = s;
        } else if ( step > s ) {
            assert(false); // never expected we loose step
        }
    }
}

Int part2( const string & line, Int t = 0 ) {
    const auto && bp = readBusPosList( readBuses( line ), false );
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
    cout << "Answer 1: " << answer1 << endl; 
    assert( answer1 == 296 );

    // Part 2
    if ( part2("17,x,13,19") != 3'417 ) assert(false);
    if ( part2("67,7,59,61" ) != 754'018 ) assert(false);
    if ( part2("67,x,7,59,61" ) != 779'210 ) assert(false);
    if ( part2("67,7,x,59,61" ) != 1'261'476 ) assert(false);
    if ( part2("1789,37,47,1889" ) != 1'202'161'486 ) assert(false);

    const BusPosList && busPos = readBusPosList( buses, true );
    const auto answer2 = part2(busPos, 0 );
    cout << "Answer 2: " << answer2 << endl;
    assert( answer2 >= 100'000'000'000'000 );
    assert( answer2 == 535296695251210 );

    return 0;
}
