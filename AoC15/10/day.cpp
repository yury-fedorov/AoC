#include <iostream>
#include <sstream>
#include <catch2/catch.hpp>

using namespace std;

string transform( string input ) {
    stringstream ss;
    int counter = 0;
    char previous = '\0';
    input += '?'; // to simplify algorithm, this symbols is never counted
    for ( char ch : input ) {
        if ( previous != ch ) {
            if ( counter > 0 ) {
                ss << counter << previous;
            }
            previous = ch;
            counter = 1;
        } else {
            counter++;
        }
    }
    return ss.str();
}

string transform_n( string input, int times ) {
    for ( int i = times; i-- > 0; ) {
        input = transform(input);
    } 
    return input;
}

TEST_CASE( "Day10", "[10]" ) {
    string input = "3113322113";
    input = transform_n(input, 40);
    REQUIRE( 329356 == input.length() );
    REQUIRE( 4666278 == transform_n(input, 50-40).length() );
}
