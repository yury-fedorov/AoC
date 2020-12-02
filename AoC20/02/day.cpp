#include <iostream>
#include <algorithm>
#include <fstream>
#include <regex>

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
            const char symbol = what[3].str()[0];
            const string pwd = what[4];
            if ( isFirstAnswer) {
                const auto count = count_if(cbegin(pwd), cend(pwd), [symbol](char ch) { return symbol == ch; } );
                validPasswordCount += count >= min && count <= max;
            } else {
                const auto match_at = [symbol, pwd](int oneBasedIndex) { return symbol == pwd[oneBasedIndex-1]; };
                validPasswordCount += ( match_at(min) + match_at(max) ) == 1;
            }
            // cout << min << " - " << max << " " << symbol << " : " << pwd << endl;
        } else {
            cerr << "Unexpected line: " << line << endl;
        }
    }
    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << validPasswordCount << endl;
    return 0;
}
