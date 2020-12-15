#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <regex>
#include <assert.h>

using namespace std;

int main() {
    vector<int> s; // sequence
    const bool isFirstAnswer = false;

    ifstream f("input.txt");
    string line;
    f >> line;
    regex re("(\\d+)");
    smatch res;
    string::const_iterator searchStart( line.cbegin() );
    while ( regex_search( searchStart, line.cend(), res, re ) ) {
        const auto n = stoi(res[1]);
        s.push_back( n );
        searchStart = res.suffix().first;
    }

    const int index = isFirstAnswer ? 2020 : 30'000'000;
    map<int,int> lastMap;
    for ( int i = 0; i < s.size() - 1; i++ ) {
        lastMap[s[i]] = i;
    }
    int last = *s.rbegin();
    for ( int i = s.size(); i < index; i++ ) {
        const auto j = lastMap.find( last );
        const int next = ( j == lastMap.end() ) ? 0 : ( i - j->second - 1 );
        lastMap[last] = i - 1; // previous element
        if ( i >= index ) break;
        last = next;
    }
    const int answer = last;
    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << answer << endl;
    assert( answer == isFirstAnswer ? 1015 : 201 );
    return 0;
}
