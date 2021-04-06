#include <iostream>
#include <set>
#include <vector>
#include <numeric>
#include <fstream>
#include <assert.h>
#include <climits>
#include <catch2/catch.hpp>

using namespace std;

typedef unsigned long long Int;
typedef vector<int> Input;
typedef set<int> Combination;
typedef set< Combination > Combinations;

Int quantumEntanglement( const Combination & c ) {
    Int result { 1 };
    for ( const int i : c ) { result *= i; }
    return result;
}

int sum( const Combination & c ) {
    int sum = 0;
    return accumulate(c.cbegin(), c.cend(), sum);
}

Combinations combinations( const Input & input, const int in, const int n, const int target ) {
    Combinations result;
    for ( int i = 0; i < in; i++ ) {
        const int e = input[i];
        if ( n > 1 ) {
            const auto target1 = target - e;
            if ( target1 > 0 ) {
                const auto && c1 = combinations( input, i, n - 1, target1 );
                for ( const auto & c1i : c1 ) {
                    Combination c2 { c1i };
                    c2.insert( e );
                    result.insert( c2 );
                }
            }
        } else if ( n == 1 ) {
            if ( target == e ) result.insert( { e } );
        } else assert( false );
    }
    return result;
}

auto day24(const bool isFirstAnswer) {
    ifstream f("24/input");
    Input input;
    string line;
    int sum {0};
    while (getline(f, line)) {
        const auto e = stoi(line);
        input.push_back(e);
        sum += e; 
    }
    const auto dt = div( sum, isFirstAnswer ? 3 : 4 );
    assert( dt.rem == 0 );
    const auto target = dt.quot;
    const int n = input.size();
    int a {0};
    int minCount = 0;
    for ( auto i = input.rbegin(); a <= target; i++ ) {
        a += *i;
        minCount++;
    }

    Int answer = 0;
    for ( int s = minCount; s < n; s++ ) {
        const auto && c = combinations(input, n, s, target);
        if ( !c.empty() ) {
            answer = ULLONG_MAX;
            for ( const auto & ci : c ) {
                answer = min( answer, quantumEntanglement(ci) );
            }
            break;
        }
    }
    return answer;
}

TEST_CASE( "Day24", "[24]" ) {
    REQUIRE( 11846773891 == day24(true)  );
    REQUIRE( 80393059    == day24(false) );
}