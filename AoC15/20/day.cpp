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

vector<int> PRIME = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97 };

bool isPrime( const int n ) {
    if ( n <= 1 ) return false;
    if ( binary_search( PRIME.cbegin(), PRIME.cend(), n ) ) return true;

    const int m = *PRIME.rbegin();
    if ( n <= m ) return false;

    for ( const int p : PRIME ) {
        if ( n % p == 0 ) return false;
    }

    const int maxI = n > 1 ;
    for ( int i = m + 1; i <= maxI; i += 2 ) {
        if ( n % i == 0 ) return false;
    }
    PRIME.push_back(n);
    return true;
}

Int presents( Int house, bool toPrint = false ) {
    map<int,int> primeCount;
    for ( Int k = 2; k <= house; k++ ) {
        while ( house >= k ) {
            const auto dt = div( house, k );
            if ( dt.rem == 0 ) {
                house = dt.quot;
                ++primeCount[k];
            } else {
                break;
            }
        }
    }
    Int count = 1;
    for ( const auto [ k, c ] : primeCount ) {
        if ( toPrint ) cout << k << " * " << c << endl;
        count += sum( k, c );
    }
    return count;
}

Int presents( Int house, const vector<int> & primes ) {
    map<int,int> primeCount;
    for ( Int k : primes ) {
        if (  k > house ) break;
        while ( house >= k ) {
            const auto dt = div( house, k );
            if ( dt.rem == 0 ) {
                house = dt.quot;
                ++primeCount[k];
                continue;
            }
            break;
        }
    }
    Int count = 1;
    for ( const auto [ k, c ] : primeCount ) {
        count += sum( k, c );
    }
    return count;
}

int main() {
    const auto r = presents( 2097152, true );
    cout << r << endl;
    return 0;

    const bool isFirstAnswer = false;

    const int TARGET { 34'000'000 };
    const int TARGET1 { TARGET / 10 };
    
    // cout << presents(181613, true) << endl;

    /*
    cout << presents( 59 ) << endl;
    cout << presents( 60, true ) << endl;
    */

    /* generation of primes
    for ( int i = 1; i <= TARGET1; i += 2 ) {
        if ( isPrime( i ) ) {
            cout << i << endl;
        }
    }
    */
    vector<int> primes;
    ifstream f("prime.txt");
    string line;
    while (getline(f, line)) {
        primes.push_back( stoi(line) );
    }

    // 2^11 - too much
    const int tooHigh = 2'097'152;
    {
        for ( int i = tooHigh; i < 6'799'994; i++ ) {
            const auto p = presents( i, primes );
            if ( p >= TARGET1 ) {
                cout << "A1: " << i << " -> " << p << endl;
                break;
            }
        }
    }

    return 0;
    

    int minDiff = TARGET1;
    int m = 0;
    int mi = 0;
    // 2097152 - That's not the right answer
    // for ( int i = 1843; i < tooHigh; i++ ) {
    for ( int i = 2'097'152; i <= 6'799'994; i++ ) {
        const auto p = presents( i, primes );
        if ( p == TARGET1 ) {
            cout << "A1: " << i << endl;
            return 0;
        }
        if ( p > m ) {
            m = p;
            mi = i;
        }
        const int diff = abs( p - TARGET1 );
        minDiff = min( minDiff, diff );
        if ( i % 10000 == 0 ) {
            cout << i << " " << p << " p(" << mi << ")=" << m << " " << minDiff << endl;
        }
    }

    return 0;
    
    auto rest = TARGET1 - 1;
    cout << presents(rest, true) << endl;

    set<Int> options;

    for ( const int p0 : primes ) {
        const auto r1 = rest - p0;
        if ( r1 < 0 ) break;
        if ( binary_search( primes.cbegin(), primes.cend(), r1 )) {
            const auto a1 = p0 * r1;
            options.insert( a1 );
            cout << p0 << " " << r1 << " " << a1 << " " << presents( a1, false ) << endl; // 6'799'994 - too high
        }
    }

    cout << "THREE" << endl;
    for ( const int p0 : primes ) {
        if ( p0 > tooHigh ) break;
        const auto r1 = rest - p0;
        if ( r1 < 0 ) break;
        for ( const int p1 : primes ) {
            if ( p1 > p0 ) break;
            const auto r2 = r1 - p1;
            if ( r2 < 0 ) break;
            if ( p0 * p1 >= tooHigh ) continue;
            // if ( r2 > p1 ) break;
            if ( binary_search( primes.cbegin(), primes.cend(), r2 )) {
                const Int a1 = (Int)(p0 * p1) * r2;
                if ( a1 >= tooHigh ) continue;
                if ( a1 < 0 ) continue;
                if ( options.find( a1 ) == options.end() ) {
                    options.insert(a1);
                    cout << a1 << " " << presents( a1, false ) << " " << *min_element( options.cbegin(), options.cend() ) << endl;
                }
            }
        }
    }

    cout << "A1: " << *min_element( options.cbegin(), options.cend() ) << endl;

/*

    for ( int n = 2; n < TARGET1; n++ ) {
        auto rest = TARGET1 - 1;
        for ( )
    }
*/
    return 0;

    for ( int i : PRIME ) {
        const auto a = TARGET1 - 1 - i;
        if ( isPrime(a) ) {
            cout << i << " * " << a << endl;
            const Int h = a * i;
            const int p = presents(h);
            cout << h << " -> " << p << endl;
            if ( p == TARGET1 ) {
                cout << "Answer 1: " << h << endl;
                return 0;
            }
        }
    }

    cerr << "Not found" << endl;
    return 0;

    presents(TARGET1 - 1, true);
    presents(TARGET1 + 1, true);
    return 0;

    for ( int i = 1; i <= 10; i++ ) {
        cout << i << " " << presents(i) << endl;
    };
    /*
    int minDiff = TARGET1;
    int m = 0;
    int mi = 0;
    for ( int i = 3390000; i < TARGET; i++ ) {
        const int p = presents( i );
        if ( p == TARGET1 ) {
            cout << "Answer 1: " << p << endl;
            break;
        }
        if ( p > m ) {
            m = p;
            mi = i;
        }
        const int diff = abs( p - TARGET1 );
        minDiff = min( minDiff, diff );
        if ( i % 10000 == 0 ) {
            cout << i << " " << p << "p(" << mi << ")=" << m << " " << minDiff << endl;
        }
    }
    */

    // cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? 1 : 2 ) << endl;

    return 0;
}
