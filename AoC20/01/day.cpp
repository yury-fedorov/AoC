#include <iostream>
#include <vector>
#include <fstream>
#include <catch2/catch.hpp>

using namespace std;

long long answer1( const vector<int> & numbers ) {
    const auto n = numbers.size();
    for ( auto i = 0; i < n; i++ ) {
        const auto a = numbers[i];
        for ( auto j = 0; j < i; j++ ) {
            const auto b = numbers[j];
            if ( ( a + b ) == 2020 ) {
                return a * b;
            }
        }
    }
    throw domain_error( "answer is not found" );
}

long long answer2( const vector<int> & numbers ) {
    const auto n = numbers.size();
    for ( auto i = 0; i < n; i++ ) {
        const auto a = numbers[i];
        for ( auto j = 0; j < i; j++ ) {
            const auto b = numbers[j];
            for ( auto k = 0; k < j; k++ ) {
                const auto c = numbers[k];
                if ( ( a + b + c ) == 2020 ) {
                    return a * b * c;
                }
            }
        }
    }
    throw domain_error( "answer is not found" );
}

TEST_CASE( "Day01", "[01]" ) {
    ifstream f("01/input.txt");
    vector<int> numbers;
    string line;
    while (getline(f, line)) {
        numbers.push_back( stoi(line) );
    }

    SECTION( "01-1" ) {
        REQUIRE( answer1(numbers) == 744475 );
    }

    SECTION( "01-2" ) {
        REQUIRE( answer2(numbers) == 70276940 );
    }
}
