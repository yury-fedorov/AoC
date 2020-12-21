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

int main() {

    const bool isFirstAnswer = false;

    ifstream f("input.txt");

    string line;
    while (getline(f, line)) {
        /* regex draft
        regex re("(\\d+)-(\\d+) ([a-z]): ([a-z]+)");
        smatch what;
        if( regex_match( line, what, re )) {
            const auto & a = what[1];
        } else {
            cerr << "Unexpected line: " << line << endl;
        }
        */
    }

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? 1 : 2 ) << endl;

    return 0;
}
