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
    auto validPasswordCount = 0;

    ifstream f("input.txt");

    regex re("(\\d+)-(\\d+) ([a-z]): ([a-z]+)");
    string line;
    while (getline(f, line)) {
        smatch what;
        if( regex_match( line, what, re )) {
            const auto min = stoi(what[1]);
            const auto max = stoi(what[2]);
            const string symbolString = what[3];
            char symbol = symbolString[0];
            const string pwd = what[4];
            auto count = 0;
            if ( isFirstAnswer) {
                // answer 1
                for ( char ch : pwd ) {
                    count += ( symbol == ch ) ? 1 : 0;
                }
                validPasswordCount += ( count >= min && count <= max ) ? 1 : 0;
            } else {
                // answer 2
                count += symbol == pwd[min-1] ? 1 : 0;
                count += symbol == pwd[max-1] ? 1 : 0;
                validPasswordCount += count == 1 ? 1 : 0;
            }

            cout << min << " - " << max << " " << symbol << " : " << pwd << endl;
        } else {
            cerr << "Unexpected line: " << line << endl;
        }
    }

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << validPasswordCount << endl;

    return 0;
}
