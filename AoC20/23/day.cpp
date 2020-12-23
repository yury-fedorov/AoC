#include <iostream>
#include <list>
#include <algorithm>
#include <assert.h>

using namespace std;

typedef int Cup;
typedef list<Cup> Cups;

void print( const Cups & cups ) {
    for ( const auto c : cups ) cout << c << " ";
    cout << endl;
}

string answer( const Cups & cups ) {
    string result;
    auto i = find( cups.cbegin(), cups.cend(), 1 );
    for ( auto j = next(i); j != i; j++ ) {
        if (j == cups.cend() ) j = cups.cbegin();
        result += to_string(*j);
    }
    return result;
}

void play( Cups & cups, int n ) {
    const bool isPrintOn = n <= 100;
    auto cur = cups.begin();
    for ( int i = 1; i <= n; i++ ) {
        if ( i % 100'000 == 0 ) cout << i << endl;
        if ( isPrintOn ) {
            cout << endl << "Move: " << i << endl;
            cout << "Cups: "; print(cups);
            cout << "Current: " << *cur << endl;
        }
        Cups pickUp;
        for ( int j = 0; j < 3; j++ ) {
            auto cur1 = next(cur);
            if ( cur1 == cups.end() ) cur1 = cups.begin();
            pickUp.push_back( *cur1 );
            cups.erase( cur1 );
        }
        if ( isPrintOn ) { cout << "Pick up: "; print(pickUp); }
        for ( int prev = *cur - 1; true; prev-- ) {
            if ( prev < 1 ) prev = *max_element( cups.cbegin(), cups.cend() );
            auto i = find( cups.begin(), cups.end(), prev );
            if ( i != cups.end() ) {
                auto j = next(i);
                if ( isPrintOn ) cout << "Destination: " << prev << endl;
                cups.insert( j, pickUp.begin(), pickUp.end() );
                break;
            }
        }
        cur++;
        if ( cur == cups.end() ) cur = cups.begin();
    }
}

int main() {

    const bool isFirstAnswer = true;

    // const string input = "463528179";
    // const string input = "32415"; // test
    // const string input = "389125467"; // test1

    const string input = "123456789";

    Cups cups;
    for ( const char ch : input ) {
        cups.push_back( ch - '0' );
    }
    
    if ( isFirstAnswer ) {
        play( cups, 10 );
        const auto answer1 = answer(cups) ;
        cout << "Answer 1: " << answer1 << endl;
        assert( answer1 == "52937846" );
    } else {
        int i = *max_element( cups.cbegin(), cups.cend() ) + 1;
        const int n = 1'000'000;
        for ( ; i <= n; i++ ) { cups.push_back(i); }
        assert( cups.size() == n );
        play( cups, 10'000'000 );
        auto j = find( cups.cbegin(), cups.cend(), 1 );
        if ( ++j == cups.cend() ) j = cups.cbegin();
        const unsigned long long a = *j;
        if ( ++j == cups.cend() ) j = cups.cbegin();
        const unsigned long long b = *j;
        const unsigned long long answer2 = a * b;
        cout << "Answer 2: " << answer2 << endl;
    }
    return 0;
}