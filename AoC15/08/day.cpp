#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <boost/regex.hpp>

using namespace std;
using namespace boost;

int main() {

    const bool isFirstAnswer = false;

    ifstream f("input.txt");
    
    regex re("(.+) -> (\\w+)");
    smatch what;

    string line;
    while (getline(f, line)) {
        if( regex_match( line, what, re, match_extra )) {
        } else {
            cerr << "Failed to parse line: " << line;
        }
    }

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << "" << endl; 

    return 0;
}
