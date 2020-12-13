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
    f >> line;
    const int t0 = stoi(line);
    cout << "t0="  << t0 << endl;
    f >> line;
    vector<int> buses;
    regex re("(\\d+)");
    smatch res;
    string::const_iterator searchStart( line.cbegin() );
    while ( regex_search( searchStart, line.cend(), res, re ) )
    {
        const int busId = stoi(res[1]);
        buses.push_back( busId );
        searchStart = res.suffix().first;
        cout << busId << endl;
    }
    cout << "CALC" << endl;
    int bestBus = 0;
    int bestTime = INT_MAX;
    for ( const int b : buses ) {
        const int wait = b - (t0 % b);
        cout << "Bus " << b << " " << wait << endl;
        if (bestTime > wait) {
            cout << "Better" << endl;
            bestTime = wait;
            bestBus = b;
        }
    }

    cout << bestBus << ',' << bestTime << '=' << bestBus * bestTime << endl; // 296 - answer 1

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? 1 : 2 ) << endl;

    return 0;
}
