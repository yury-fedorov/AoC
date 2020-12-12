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

typedef long long Int;

Int sum( int n, int c ) {
    Int last = 1;
    Int result = 0;
    for ( auto i = 0; i < c; i++ ) {
        last *= n;
        result += last;
    }
    return result;
}

Int presents( int house ) {
    // cout << "p(" << house << endl;
    map<int,int> primeCount;
    // primeCount[house] = 1;
    primeCount[1] = 1;
    // const int maxK = house >> 1;  // devide by 2
    for ( auto k = 2; k <= house; k++ ) {
        while ( house >= k ) {
            const auto dt = div( house, k );
            if ( dt.rem == 0 ) {
                // cout << house << " " << k << " " << dt.quot << " " << dt.rem << endl;
                house = dt.quot;
                ++primeCount[k];
            } else {
                break;
            }
        }
    }
    Int count = 0;
    for ( const auto [ k, c ] : primeCount ) {
        // cout << k << " " << c << endl;
        count += sum( k, c );
    }
    return count;
}

int main() {
    const bool isFirstAnswer = false;

    const Int TARGET { 34000000 };
    const Int TARGET1 { TARGET / 10 };

    for ( int i = 1; i < TARGET1; i++ ) {
        const int p = presents( i );
        if ( p == TARGET1 ) {
            cout << "Answer 1: " << p << endl;
            break;
        }
        if ( i % 10000 == 0 ) {
            cout << i << endl;
        }
    }

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? 1 : 2 ) << endl;

    return 0;
}
