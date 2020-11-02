#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>

using namespace std;

const vector<string> notNiceList = { "ab", "cd", "pq", "xy" };
const string vowels = "aeiou";

bool isNice( const string & s ) {
    for ( const auto & notNice : notNiceList ) {
        if ( s.find( notNice ) != string::npos ) return false;
    }
    bool isDouble = false;
    auto prev = ' ';
    vector<char> vowelsInside;
    for (const auto cur : s) {
        if ( cur == prev ) {
            isDouble = true;
        }
        if (vowels.find(cur)!= string::npos) {
            vowelsInside.push_back(cur);
        }
        prev = cur;
    }
    return isDouble && vowelsInside.size() >= 3;
}

bool isNice2( const string & s ) {
    const int l = s.length();

    bool isAbA = false;
    for ( int i = 0; i < l - 2; i++ ) {
        if ( s[i] == s[i+2] ) {
            isAbA = true;
            break;
        }
    }
    if (!isAbA) return false;

    for ( int i = 0; i < l - 3; i++ ) {
        for ( int j = i + 2; j < l; j++ ) {
            if ( s[i] == s[j] && s[i+1] == s[j+1] ) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    ifstream f("input.txt");
    
    auto niceCount(0);
    auto niceCount2(0);
    string line;
    while (getline(f, line)) {
        niceCount += isNice(line) ? 1 : 0;
        niceCount2 += isNice2(line) ? 1 : 0;
    }

    // Question 1
    cout << "Answer 1: " << niceCount << endl;

    // Question 2
    cout << "Answer 2: " << niceCount2 << endl;

    return 0;
}
