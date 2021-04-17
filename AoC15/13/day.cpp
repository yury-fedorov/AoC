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
#include <catch2/catch.hpp>

using namespace std;

typedef tuple<string,bool,int,string> Fact;
typedef pair<int,int> Pair;
typedef map<Pair,int> FactMap;

inline Pair makePair( int a, int b ) { return { min(a,b), max(a,b) }; }

int countHappiness( const FactMap & facts, const vector<int> & positions ) {
    int happiness = 0;
    const int n = positions.size();
    for ( auto i = 0; i < n; i++ ) {
        const auto j = (i + 1) % n;
        const auto a = positions[i];
        const auto b = positions[j];
        const auto h = facts.find( makePair( a, b ) );
        if ( h != facts.end() ) {
            happiness += h->second;
        }
    }
    return happiness;
}

int day13( const bool isFirstAnswer ) {
    ifstream f("13/input.txt");

    vector<Fact> facts;

    // Alice would gain 54 happiness units by sitting next to Bob.
    // Alice would lose 79 happiness units by sitting next to Carol.
    regex re("(\\w+) would (\\w+) (\\d+) happiness units by sitting next to (\\w+).");
    string line;
    while (getline(f, line)) {      
        smatch what;
        if( regex_match( line, what, re )) {
            facts.emplace_back( what[1], what[2] == "gain", stoi(what[3]), what[4] );
        } else {
            cerr << "Unexpected line: " << line << endl;
            FAIL();
        }
    }

    set<string> persons;
    for ( const auto & f : facts ) {
        persons.insert(get<0>(f));
    }

    const auto ME = "me";
    if (!isFirstAnswer) {
        persons.insert(ME); // for the part 2
    }

    const int personCount = persons.size();
    const auto adjPersonCount = isFirstAnswer ? personCount : personCount - 1;

    // now we optimize 
    FactMap factsMap;
    for ( auto i = 0; i < adjPersonCount; i++ ) {
        for ( auto j = i + 1; j < adjPersonCount; j++ ) {
            // get names by indexes
            const string & nameA = *next(persons.cbegin(), i);
            const string & nameB = *next(persons.cbegin(), j);
             // find both facts in vector
            auto ab = find_if( facts.cbegin(), facts.cend(), 
                [nameA, nameB] (const Fact & f) { return get<0>(f) == nameA && get<3>(f) == nameB; } );
            auto ba = find_if( facts.cbegin(), facts.cend(), 
                [nameA, nameB] (const Fact & f) { return get<0>(f) == nameB && get<3>(f) == nameA; } );

            // sum the happiness
            auto fh = []( const Fact & f ) { return ( get<1>(f) ? 1 : -1 ) * get<2>(f); };
            const int happiness = fh(*ab) + fh(*ba);
            // cout << nameA << " & " << nameB << " " << fh(*ab) << " + " << fh(*ba) << " = " << happiness << endl;    
            factsMap.insert( make_pair( makePair(i,j), happiness ) );
        }
    }

    vector<int> positions(personCount);
    iota( positions.begin(), positions.end(), 0 );

    // first position we always keep fix as soon as it is round table
    const auto secondPosition = positions.begin() + 1;
    // we also need to ignore direction of count (it gives us another reduction of space by 2)
    // to have such filter we consider that the second position (person on the right from the first) 
    // is always smaller than the last one (person on the left from the first)
    // 1 2 ... 8 -> is ok
    // 1 8 ... 2 -> is filtered out
    const auto lastPositionIndex = personCount - 1;
    int maxHappiness = INT_MIN;
    do {
        const auto right = positions[1];
        const auto left = positions[lastPositionIndex];
        if ( right < left ) {
            const auto curHappiness = countHappiness(factsMap, positions);
            maxHappiness = max(maxHappiness, curHappiness);
        }
    } while ( next_permutation( secondPosition, positions.end() ) );

    return maxHappiness;
}

TEST_CASE( "Day13", "[13]" ) {
    REQUIRE( 618 == day13(true) );
    REQUIRE( 601 == day13(false) );
}