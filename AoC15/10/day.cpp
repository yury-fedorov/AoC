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

int main() {
    string input = "3113322113";
    input = transform_n(input, 40);
    cout << "Answer 1: " << input.length() << endl;
    cout << "Answer 2: " << transform_n(input, 50-40).length() << endl;
    return 0;
}
