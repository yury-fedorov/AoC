#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <regex>
#include <catch2/catch.hpp>

using namespace std;

typedef pair<string, int> Fact;
typedef map<string, int> Facts;
typedef map<int,Facts> SueFacts;

optional<int> get( const Facts & facts, const string & fact) {
    const auto i = facts.find( fact );
    return ( i == facts.end() ) ? optional<int>() : optional<int>{ i->second };
}

auto day16( const bool isFirstAnswer ) {
    SueFacts facts;

    ifstream f("16/input.txt");
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

    const map<string, function<bool(int,int)> > notExact { {"cats", greater<int>()}, {"trees", greater<int>()}, 
        {"pomeranians", less<int>()}, {"goldfish", less<int>() } };

    for ( const auto & ck : known ) {
        SueFacts good;
        const auto nei = notExact.find( ck.first ) ;
        const auto match = ( isFirstAnswer || nei == notExact.end() ) ? equal_to<int>() : nei->second;
        copy_if(facts.cbegin(), facts.cend(), inserter(good, good.end()), 
            [ck, match](const pair<int,Facts> & p) { 
                const auto r = get( p.second, ck.first );
                return r.has_value() ? match( r.value(), ck.second ) : true;
            } );
        
        const auto n = good.size();
        if ( n == 1 ) {
            return good.begin()->first;
        }
        swap( facts, good );
    }
    throw new domain_error( "failed to find a solution" );
}

TEST_CASE( "Day16", "[16]" ) {
    REQUIRE( 40 == day16(true) );
    REQUIRE( 241  == day16(false));
}