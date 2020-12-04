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

typedef tuple<string,bool,int,string> Fact;
typedef pair<int,int> Pair;
typedef map<Pair,int> FactMap;

Pair makePair( int a, int b ) {
    return make_pair( min(a,b), max(a,b) );
}

int countHappiness( const FactMap & facts, const vector<int> & positions ) {
    auto happiness = 0;
    const auto n = positions.size();
    for ( auto i = 0; i < n; ) {
        const auto j = ++i % n;
        const auto & h = facts.find( makePair( i, j ) );
        assert( h != facts.end() );
        happiness += h->second;
    }
    return happiness;
}

int main() {

    const bool isFirstAnswer = false;

    ifstream f("input.txt");

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
        }
    }

    set<string> persons;
    for ( const auto & f : facts ) {
        persons.insert(get<0>(f));
    }
    const auto personCount = persons.size();
    cout << personCount << endl;

    // now we optimize 
    FactMap factsMap;
    for ( auto i = 0; i < personCount; i++ ) {
        for ( auto j = i + 1; j < personCount; j++ ) {
            // get names by indexes
            // find both facts in vector
            // sum the happiness
            // XXX to do
            factsMap.insert( make_pair( make_pair(i,j), 0 ) );
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
    auto maxHappiness = INT_MIN;
    do {
        const auto right = positions[1];
        const auto left = positions[lastPositionIndex];
        if ( right < left ) {
            const auto curHappiness = countHappiness(factsMap, positions);
            maxHappiness = max(maxHappiness, curHappiness);
        }
    } while ( next_permutation( secondPosition, positions.end() ) );

    cout << "Answer " << maxHappiness << endl;

    return 0;
}
