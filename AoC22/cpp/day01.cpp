#include "common.h"

using namespace std;

namespace day01 {
}

TEST_CASE( "Day01", "[01]" ) {
    using namespace day01;
    
    ifstream f("../data/day01.txt");
    string line;
    auto answer1 = 0; 
    auto sum = 0;
    while ( !f.eof() ) {
        getline(f, line);
        const auto value = stoi( line );
        if ( value ) sum += value;
        else { 
            answer1 = max( sum, answer1 );
            sum = 0;
        }
    }
    
    SECTION( "01-1" ) {
        REQUIRE( answer1 == 0 );
    }

    SECTION( "01-2" ) {
        REQUIRE( false );
    }
}
