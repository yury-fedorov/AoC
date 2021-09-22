#include <fstream>
#include <set>
#include "common.h"

using namespace std;

namespace day06 {
    typedef vector< pair<string,string> > Data;

    constexpr string_view Com = "COM";

    int count( const Data & data, const string & obj ) noexcept {
        if ( obj == Com ) return 0;
        const auto r = r::find_if(data, [&obj](const auto & _){ return _.second == obj; } );
        assert( data.end() == r ); // must not ever happen
        if ( r->first == Com ) return 1;
        return 1 + count( data, r->first );
    }

    int answer1( const auto & data ) {
        set<string> all;
        r::for_each( data, [&all](const auto & _) { all.insert( _.first ); all.insert( _.second ); } );
        const auto v = all | rv::transform( [&data](const auto & _) { return count(data, _); } )
            | r::to_vector;
        return r::accumulate(v, 0);
    }
    int answer2( const auto &  ) { return 0; }
}

TEST_CASE( "Day06", "[06]" ) {
    using namespace day06;

    Data data;
    ifstream f("../data/Day06.txt");
    string line;
    while ( getline(f, line) ) {
        const auto elements = split(line, ')');
        assert(elements.size() == 2);
        data.emplace_back( elements[0], elements[1] );
    }

    const auto code = split( line, ',' );
    SECTION( "06-1" ) {
        REQUIRE( answer1(data) == -1 );
    }

    SECTION( "06-2" ) {
        REQUIRE( answer2(data) == -2 );
    }
}
