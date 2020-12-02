#include <iostream>
#include <fstream>
#include <sstream>
#include <string_view>
#include <regex>

using namespace std;

int main() {
    ifstream f("input.txt");

    long long sum = 0;

    string line;
    while (getline(f, line)) {
        regex exp("(-?\\d+)");
        smatch res;
        string::const_iterator searchStart( line.cbegin() );
        while ( regex_search( searchStart, line.cend(), res, exp ) )
        {
            sum += stoi(res[0]);
            // cout << ( searchStart == line.cbegin() ? "" : " " ) << res[0];  
            searchStart = res.suffix().first;
        }
    }

    cout << sum << endl;

    return 0;
}
