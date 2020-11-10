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

    long maxDistance = 0;
    long minDistance = LONG_MAX;

    map< pair<string,string>,int > routes;
    set<string> sites;
    regex re("(.+) to (.+) = (\\d+)");

    string line;
    while (getline(f, line)) {
        smatch what;
        if( regex_match( line, what, re )) {
            const auto & a = what[1];
            const auto & b = what[2];
            const int d = stoi(what[3]);
            routes.emplace( make_pair( a, b ), d );
            sites.insert(a);
            sites.insert(b);
        } else {
            cerr << "Unexpected line: " << line << endl;
        }
    }
    const auto siteCount( sites.size() );
    cout << "Cites: " << siteCount << endl;
    vector<string> siteList(sites.begin(), sites.end());
    vector<int> route(siteCount);
    iota( route.begin(), route.end(), 0 );
  
    const auto noRoute = routes.end();

    do {
        long distance = 0;
        auto a = siteList[route[0]];
        for ( int i = 1; i < siteCount; i++ ) {
            const string & b = siteList[route[i]];
            assert( a != b );
            const auto ab = routes.find( make_pair(a, b) );
            if ( ab != noRoute ) {
                distance += ab->second;
            } else {
                const auto ba = routes.find( make_pair(b, a) );
                if ( ba != noRoute ) {
                    distance += ba->second;
                } else {
                    cerr << "No route found between " << a << " and " << b << endl; 
                }
            }
            a = b;
        }
        minDistance = min(minDistance, distance);
        maxDistance = max(maxDistance, distance);
    } while ( std::next_permutation(route.begin(),route.end()) );    

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? minDistance : maxDistance ) << endl; // 884 too high

    return 0;
}
