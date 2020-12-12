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
const int MAX_DISTANCE = 505;
int globalMin = MAX_DISTANCE;
int distance( const string & input, const ToFromRules & rules, int depth ) {
    if ( input == START ) return 0;
    if ( depth >= MAX_DISTANCE ) return MAX_DISTANCE;
    int minLength = MAX_DISTANCE;
    for ( auto & p : rules ) {
        // we go back from end to the beginning
        const string & to = p.first;
        const string & from = p.second;
        auto i = input.find( to );
        const int len = to.length();
        while ( i != string::npos ) {
            string product { input };
            product.replace( i, len, from );
            if ( product.length() < input.length() ) {
                const int d = 1 + distance( product, rules, depth + 1 );
                if ( d < minLength ) {
                    minLength = d;
                    const int localMin = depth + d;
                    if ( localMin < globalMin ) {
                        globalMin = localMin;
                        cout << globalMin << endl;
                    }
                }
            }
            i = input.find(to, i + 1 );
        }
    }
    return minLength;
}

int main() {
    ifstream f("input.txt");

    Rules rules;
    map<string,string> toFrom;
    string input;

    regex reRule("^(\\w+) => (\\w+)$");
    smatch what;
    string line;
    while (getline(f, line)) {
        if( regex_match( line, what, reRule )) {
            const string from = what[1];
            const string to = what[2];
            rules[from].push_back(to);
            toFrom.insert( make_pair( to, from ) );
        } else if ( !line.empty() ) {
            assert(input.empty());
            input = line;
        }
    }

    Combinations combinations;
    addAll( input, rules, combinations );
    cout << "Answer 1: " << combinations.size() << endl; // 535

    cout << "Answer 2: " << distance(input, toFrom, 0 ) << endl;


    return 0;
}
