#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

typedef long long lint;

bool isSum( const vector<lint> & v, lint n ) {
    auto rit = v.rbegin();
    std::advance(rit, 25);
    vector<lint> v1 (v.rbegin(), rit);
    assert( v1.size() == 25 );
    sort( v1.begin(), v1.end() );
    const auto min = *v1.begin();
    const auto max = *v1.rbegin();
    for ( auto r : v1 ) {
        const auto d = n - r;
        if ( d <= 0 || d < min || d > max || d == r ) continue;
        if ( binary_search( v1.begin(), v1.end(), d ) ) return true;
    }
    return false;
}

int main() {
    vector<lint> numbers;

    lint answer1 = 0;

    ifstream f("input.txt");
    string line;
    while (getline(f, line)) {
        const auto n = stoll(line);
        if ( numbers.size() >= 25 ) {
            if ( !isSum( numbers, n ) ) {
                answer1 = n;
                cout << "Answer 1: " << n << endl; // 29221323 - answer 1
            }
        }
        numbers.push_back(n);
    }

    const auto n = numbers.size();
    for ( int i = 0; i < n; i++ ) {
        lint sum = answer1;
        for ( int j = i; j < n; j++ ) {
            sum -= numbers[j];
            if ( sum == 0 && ( i < j ) ) { // at least 2 numbers
                // found
                lint mi = numbers[i];
                lint ma = numbers[j]; // XXX - minmax and even max for some reason did not work well
                for ( auto k = i + 1; k <= j; k++ ) {
                    const auto a = numbers[k];
                    mi = a < mi ? a : mi;
                    ma = a > ma ? a : ma;
                }
                cout << "Answer 2: " << ( mi + ma ) << endl;
            } else if ( sum < 0 ) break;
        }
    }

    return 0;
}
