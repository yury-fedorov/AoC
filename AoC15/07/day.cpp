#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <boost/regex.hpp>

using namespace std;
using namespace boost;

regex re3("(.+) (\\w+) (.+)");
regex re2("NOT (.+)");
regex red("\\d+");

map<string,string> varExp;
map<string,uint16_t> valMap;

uint16_t eval( const string & e ) {
    const auto cache = valMap.find(e);
    if ( cache != valMap.end() ) return cache->second;

    smatch what;
    if( regex_match( e, what, re3, match_extra )) {
        // a op b
        const auto a = eval(what[1] );
        const auto b = eval(what[3] );
        const auto op = what[2];
        if ( op == "AND" ) return a & b;
        if ( op == "OR" ) return a | b;
        if ( op == "LSHIFT" ) return a << b;
        if ( op == "RSHIFT" ) return a >> b;
        cerr << "Unknown expression: " << e << endl;
        return 0;
    } 
    if ( regex_match( e, what, re2, match_extra )) {
        // not a
        const auto a = eval(what[1] );
        return ~a; // betwise not
    }
    // is a number or variable?
    const bool isNumber = regex_match( e, what, red, match_extra );
    const auto result = isNumber ? stoi(e) : eval( varExp[e] );
    valMap[e] = result; // put in cache
    return result;
}

int main() {

    const bool isFirstAnswer = false;

    ifstream f("input.txt");
    
    regex re("(.+) -> (\\w+)");
    smatch what;

    string line;
    while (getline(f, line)) {
        if( regex_match( line, what, re, match_extra )) {
            varExp[what[2]] = what[1];
        } else {
            cerr << "Failed to parse line: " << line;
        }
    }

    varExp["b"] = "16076"; // answer 1

    const auto a = eval( varExp["a"] );

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << a << endl; 

    return 0;
}
