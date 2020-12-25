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

int loops( const int pk ) {
    const int subjectNumber = 7;
    int value = 1;
    for ( int loop = 0; true; loop++ ) {
        if ( value == pk ) return loop;
        value *= subjectNumber;
        value %= 20201227;
    }
    return -1;
}

long encryptionKey( const int subjectNumber, const int loops ) {
    long value = 1;
    for ( int loop = 0; loop < loops; loop++ ) {
        value *= subjectNumber;
        value %= 20201227;
    }
    return value;
}

int main() {
    vector<int> input;

    ifstream f("input.txt");
    string line;
    while (getline(f, line)) {
        input.push_back( stoi(line) );
        cout << stoi(line) << endl;
    }

    // const int pk1 = 5764801; // input[0];
    const int pk1 = input[0];
    const int l1 = loops( pk1 );

    const int pk2 = input[1];
    const int l2 = loops( pk2 );

    cout << "8? -> " << loops( 5764801 ) << endl;
    cout << "11? -> " << loops( 17807724 ) << endl;
    cout << l1 << endl;
    cout << l2 << endl;
    cout << encryptionKey(17807724, 8) << endl;
    cout << encryptionKey(pk1, l2) << endl;
    cout << encryptionKey(pk2, l1) << endl;

    cout << "Answer 1: " << encryptionKey(pk1, l2) << endl;

    return 0;
}
