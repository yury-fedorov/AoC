#include "common.h"

using namespace std;

namespace day06 {
    typedef vector< pair<string,string> > Data;

    constexpr string_view Com = "COM";

    int count( const Data & data, const string & obj ) noexcept {
        // version without caching takes 24 seconds
        static map<string,int> cache; // XXX: simplified implementation, would not work for different data
        const auto i = cache.find(obj);
        if ( i != cache.end() ) { return i->second; }

        int result = 0;
        const auto r = r::find_if(data, [&obj](const auto & _){ return _.second == obj; } );
        if ( data.end() != r ) {
            result = 1 + count( data, r->first );
        }
        cache.insert( { obj, result } );
        return result;
    }

    int answer1( const auto & data ) {
        set<string> all;
        r::for_each( data, [&all](const auto & _) { all.insert( _.first ); all.insert( _.second ); } );
        const auto v = all | rv::transform( [&data](const auto & _) { return count(data, _); } ) | r::to_vector;
        return r::accumulate(v, 0);
    }

    vector<string> path( const Data & data, string obj ) noexcept {
        vector<string> result;
        while ( obj != Com ) {
            const auto r = r::find_if(data, [&obj](const auto & _){ return _.second == obj; } );
            if ( data.end() == r ) return {}; // detached?
            result.push_back( r->first );
            obj = r->first;
        }
        return result;
    }

    auto answer2( const auto & data ) {
        auto you_path = path( data, "YOU" );
        auto san_path = path( data, "SAN" );

        int common_part = 0;
        auto ai = you_path.rbegin();
        auto bi = san_path.rbegin();
        const auto ae = you_path.rend();
        const auto be = san_path.rend();
        for ( ; ai != ae && bi != be; ai++, bi++ ) {
            if ( *ai == *bi ) common_part++;
            else break;
        }
        return you_path.size() + san_path.size() - ( 2  * common_part );
    }
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
        REQUIRE( answer1(data) == 312697 );
    }

    SECTION( "06-2" ) {
        REQUIRE( answer2(data) == 466 );
    }
}
