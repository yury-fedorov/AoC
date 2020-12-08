#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <regex>

using namespace std;

typedef pair<string, int> Fact;
typedef map<string, int> Facts;
typedef map<int,Facts> SueFacts;

optional<int> get( const Facts & facts, const string & fact) {
    auto i = facts.find( fact );
    if ( i == facts.end() ) return {};
    return i->second;
}

enum Result { NoMatch, Match, Unknown };

Result match( const Facts & facts, const string & fact, int qty ) {
    const auto r = get(facts, fact);
    return r.has_value() ? ( r.value() == qty ? Match : NoMatch ) : Unknown;
}

int main() {
    const bool isFirstAnswer = false;
    SueFacts facts;

    ifstream f("input.txt");
    // Sue 500: cats: 2, goldfish: 9, children: 8
    const regex reLine("^Sue (\\d+): (.+)$");
    const regex reList("(\\w+): (\\d+)");
    string line;
    smatch what;
    while (getline(f, line)) {
        if( regex_match( line, what, reLine)) {
            const int sueId = stoi(what[1]);
            const string all = what[2];
            Facts list;
            string::const_iterator searchStart( all.cbegin() );
            while ( regex_search( searchStart, all.cend(), what, reList ) )
            {
                list.insert( make_pair(what[1], stoi(what[2]) ) );
                searchStart = what.suffix().first;
            }
            facts.insert( make_pair( sueId, list ) );
        } else {
            cerr << "Unexpected line: " << line << endl;
        }
    }

    const vector<Fact> known { {"children", 3}, {"cats", 7}, {"samoyeds", 2}, {"pomeranians", 3}, {"akitas", 0},
        {"vizslas", 0}, {"goldfish", 5}, {"trees", 3}, {"cars", 2}, { "perfumes", 1 } };

    const map<string,bool> notExact { {"cats", true}, {"trees", true}, {"pomeranians", false}, {"goldfish", false} };

    for ( const auto & ck : known ) {
        SueFacts good;
        auto nei = notExact.find( ck.first ) ;
        if ( isFirstAnswer || nei == notExact.end() ) {
            copy_if(facts.cbegin(), facts.cend(), inserter(good, good.end()), 
                [ck](const pair<int,Facts> & p) { return match( p.second, ck.first, ck.second ); } );
        } else {
            copy_if(facts.cbegin(), facts.cend(), inserter(good, good.end()), 
                [ck, nei](const pair<int,Facts> & p) { 
                    auto r = get( p.second, ck.first );
                    return r.has_value() ? ( nei->second ? r > ck.second : r < ck.second ) : true;
                } );
        }
        const auto n = good.size();
        if ( n == 1 ) {
            cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << good.begin()->first << endl;
            break;
        }
        swap( facts, good );
    }

    return 0;
}
