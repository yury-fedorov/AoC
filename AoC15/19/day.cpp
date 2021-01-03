#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <fstream>
#include <regex>
#include <assert.h>
#include <climits>

using namespace std;

typedef map< string, vector<string> > Rules;
typedef map< string, string > ToFromRules;
typedef set<string> Combinations;

void addAll( const string & input, const Rules & rules, Combinations & combinations ) {
    for ( const auto & p : rules ) {
        const string & from = p.first;
        const auto fromLen = from.length();
        for ( const auto & to : p.second ) {
            auto i = input.find( from );
            while ( i != string::npos ) {
                string product { input };
                product.replace( i, fromLen, to );
                combinations.insert( product );
                i = input.find(from, i + 1 );
            }
        }
    }
}
const string START { "e" };
const int MAX_DISTANCE = 505 * 2; // length of the given string

int distance( const string & input, const ToFromRules & rules, const vector<string> & toList, int depth ) {
    if ( input == START ) return 0;
    if ( depth >= MAX_DISTANCE ) return MAX_DISTANCE;
    for ( const auto & to : toList ) {
        // we go back from end to the beginning
        const string & from = rules.at(to);
        auto i = input.find( to );
        const int len = to.length();
        while ( i != string::npos ) {
            string product { input };
            product.replace( i, len, from );
            if ( product.length() <= input.length() ) {
                const auto d = distance( product, rules, toList, depth + 1 );
                if ( d >= MAX_DISTANCE ) return MAX_DISTANCE;
                return 1 + d;
            }
            i = input.find(to, i + 1 );
        }
    }
    return MAX_DISTANCE;
}

int main() {
    ifstream f("input.txt");

    Rules rules;
    map<string,string> toFrom;
    vector<string> toList;
    string input;

    regex reRule("^(\\w+) => (\\w+)$");
    smatch what;
    string line;
    while (getline(f, line)) {
        if( regex_match( line, what, reRule )) {
            const string from = what[1];
            const string to = what[2];
            rules[from].push_back(to);
            toFrom.insert( { to, from } );
            toList.push_back(to);
        } else if ( !line.empty() ) {
            assert(input.empty());
            input = line;
        }
    }

    Combinations combinations;
    addAll( input, rules, combinations );
    cout << "Answer 1: " << combinations.size() << endl;
    assert( 535 == combinations.size() );

    // sort longest "to" first
    sort( toList.begin(), toList.end(), []( const string & a, const string & b ) { return a.length() > b.length(); } );
    const auto answer2 = distance(input, toFrom, toList, 0 );
    cout << "Answer 2: " << answer2 << endl;
    assert( 212 = answer2 );

    return 0;
}
