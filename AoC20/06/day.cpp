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

    auto sumUniqueAnswers = 0;
    set<char> uniqueInGroup;
    bool isNewSet = true;
    ifstream f("input.txt");
    string line;
    while (getline(f, line)) {
        if ( line.empty() ) {
            // group is over
            sumUniqueAnswers += uniqueInGroup.size();
            uniqueInGroup.clear();
            isNewSet = true; // for second question
        } else {
            set<char> one;
            const bool isOriginalSet = isFirstAnswer || isNewSet;
            set<char> & s = isOriginalSet ? uniqueInGroup : one;
            for ( char ch : line ) {
                s.insert(ch);
            }
            if (!isOriginalSet) {
                set <char> u;
                for ( const char ch : one ) {
                    if ( uniqueInGroup.find(ch) != uniqueInGroup.end() ) {
                        u.insert(ch);
                    }
                }
                swap( uniqueInGroup, u );
            }
            isNewSet = false;
        }
    }
    sumUniqueAnswers += uniqueInGroup.size();

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << sumUniqueAnswers << endl; // 3471 - not right 

    return 0;
}
