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

int main() {
    const bool isFirstAnswer = true;

    ifstream f("input.txt");

    map< string, vector<string> > rules;
    regex reRule("^(\\w+) => (\\w+)$");
    smatch what;
    string line;

    string input;

    while (getline(f, line)) {
        if( regex_match( line, what, reRule )) {
            const string from = what[1];
            const string to = what[2];
            rules[from].push_back(to);
        } else if ( !line.empty() ) {
            assert(input.empty());
            input = line;
        }
    }

    cout << rules.size() << endl;

    set<string> replacements;
    for ( const auto & p : rules ) {
        const string & from = p.first;
        cout << from << " -> " << endl;
        const auto fromLen = from.length();
        for ( const auto & to : p.second ) {
            cout << to << endl;
            auto i = input.find( from );
            while ( i != string::npos ) {
                string product { input };
                product.replace( i, fromLen, to );
                replacements.insert( product );
                i = input.find(from, i + 1 );
            }
        }
    }

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? replacements.size() : 2 ) << endl;

    return 0;
}
