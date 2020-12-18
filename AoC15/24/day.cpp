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

typedef unsigned long long Int;
typedef vector<int> Input;
typedef set<int> Combination;
typedef set< Combination > Combinations;

Int quantumEntanglement( const Combination & c ) {
    Int result { 1 };
    for ( const int i : c ) { result *= i; }
    return result;
}

auto sum( const Combination & c ) {
    int sum = 0;
    return accumulate(c.cbegin(), c.cend(), sum);
}

Combinations combinations( const Input & input, const int n, const int target ) {
    const auto in1 = input.size() - 1; 
    Combinations result;
    if ( target > 0 ) {
        for ( const int e : input ) {
            if ( n > 1 ) {
                const auto target1 = target - e;
                if ( target1 > 0 ) {
                    Input copy( in1 ); // to reduce number of combinations we take only smaller elements
                    for ( const int e1 : input ) {
                        if ( e1 < e ) copy.push_back(e1);
                        else break; 
                    }
                    const auto && c1 = combinations( copy, n - 1, target1 );
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
    }
    return result;
}

int main() {
    const bool isFirstAnswer = true;
    ifstream f("input");
    Input input;
    string line;
    int sum {0};
    while (getline(f, line)) {
        const auto e = stoi(line);
        input.push_back(e);
        sum += e; 
    }
    const auto dt = div( sum, 3 );
    assert( dt.rem == 0 );
    const auto target = dt.quot;
    const auto n = input.size();
    int a {0};
    int minCount = 0;
    for ( auto i = input.rbegin(); a <= target; i++ ) {
        a += *i;
        minCount++;
    }

    cout << target << " " << n << " " << minCount << endl;

    for ( int s = minCount; s < n; s++ ) {
        cout << "Attempt with elements: " << s << endl;
        const auto && c = combinations(input, n, target);
        if ( !c.empty() ) {
            cout << "Found size: " << s << endl; // XXX: not sure because other 2 groups must split exactly too
            if ( c.size() == 1 ) {
                cout << "Unique solution: " << quantumEntanglement( *c.begin() ) << endl;
            } else {
                Int mi {0};
                for ( const auto & ci : c ) {
                    mi = min( mi, quantumEntanglement(ci) );
                }
                cout << "QE: " << mi << endl;
            }
            break;
        }
    }

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? dt.quot : 2 ) << endl;

    return 0;
}
