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

typedef map<string, vector< pair<string,int> > > Rules;

bool canContain( const Rules & rules, const string & out, const string & in ) {
    const auto found = rules.find(out);
    if (found == rules.end()) {
        cerr << out << " - " << in << endl;
        assert(false);
    } 
    for ( const auto & p : found->second ) {
        if ( p.first == in ) return true;
        if ( canContain(rules, p.first, in ) ) return true;
    }
    return false;
}

int main() {

    const bool isFirstAnswer = true;

    ifstream f("input.txt");

    string line;

    // drab tan bags contain 4 clear gold bags.
    // vibrant lime bags contain 3 faded gold bags, 3 plaid aqua bags, 2 clear black bags.
    const regex reLine("^([\\w ]+) bags contain ([^.]+).$");
    const regex reList("([\\d]+) ([\\w ]+) bag");
    Rules rules;

    while (getline(f, line)) {
        smatch what;
        if( regex_match( line, what, reLine)) {
            const string container = what[1];
            const string all = what[2];

            vector< pair<string,int> > list;

            string::const_iterator searchStart( all.cbegin() );
            while ( regex_search( searchStart, all.cend(), what, reList ) )
            {
                list.emplace_back( what[2], stoi(what[1]) );
                searchStart = what.suffix().first;
            }
            rules.insert( make_pair( container, list ) );
        } else {
            cerr << "Unexpected line: " << line << endl;
        }
    }

    cout << rules.size() << endl;

    auto count = 0;
    const string color = "shiny gold";
    for ( auto & p : rules ) {
        count += canContain( rules, p.first, color);
    }

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? count : 2 ) << endl;

    return 0;
}
