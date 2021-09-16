#include <iostream>
#include <fstream>
#include <catch2/catch.hpp>

using namespace std;

namespace day05 {
    long long answer1( const string_view ) {
        throw domain_error( "answer is not found" );
    }

    long long answer2( const string_view ) {
        throw domain_error( "answer is not found" );
    }
}

TEST_CASE( "Day05", "[05]" ) {
    using namespace day05;
    
    ifstream f("../data/Day05.txt");
    string line;
    getline(f, line);

    SECTION( "05-1" ) {
        REQUIRE( answer1(line) == 744475 );
    }

    SECTION( "05-2" ) {
        REQUIRE( answer2(line) == 70276940 );
    }
}
