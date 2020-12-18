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
#include <string_view>

using namespace std;

typedef unsigned long long Int;

Int evaluate( const string & e ) {
    stack<string> s;
    string level;
    for ( const char ch : e ) {
        if ( ch == '(' ) {
            s.push(level);
            level = "";
        } else if ( ch == ')' ) {
            const string level1 = s.top();
            s.pop();
            const auto value = evaluate( level );
            level = level1 + to_string(value);
        } else {
            level += ch;
        }
    }
    assert(s.empty());
    // qui level is flat: no parantesis
    cout << level << endl;
    static const regex re("([^ ]+)");
    smatch res;
    string::const_iterator searchStart( level.cbegin() );
    while ( regex_search( searchStart, level.cend(), res, re ) )
    {
        const string term = res[1];
        const bool isOperator = term == "+" || term == "*";
        if ( isOperator || s.size() < 2 ) {
            s.push(term); // +1
        } else {
            // neither operator, nor first argument, we can calculate
            const auto arg2 = stoull(term); 
            const bool isSum = s.top() == "+";
            s.pop();
            const auto arg1 = stoull(s.top());
            s.pop();
            const auto r = isSum ? ( arg1 + arg2 ) : ( arg1 * arg2 );
            s.push( to_string(r) ); // -1
        }
        searchStart = res.suffix().first;
    }
    assert( s.size() == 1 );
    const string & result = s.top();
    cout << result << endl;
    return stoull(result);
}

int main() {

    const bool isFirstAnswer = true;

    Int sum {0};

    ifstream f("input");

    string line;
    while (getline(f, line)) {
        cout << line << endl;
        const auto r = evaluate(line);
        sum += r;
        cout << r << endl;
    }

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? sum : 2 ) << endl;

    return 0;
}
