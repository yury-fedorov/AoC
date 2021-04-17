#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <regex>
#include <catch2/catch.hpp>

using namespace std;

regex re3("(.+) (\\w+) (.+)");
regex re2("NOT (.+)");
regex red("\\d+");

map<string,string> varExp;
map<string,uint16_t> valMap;

uint16_t eval( const string & e ) {
    const auto cache = valMap.find(e);
    if ( cache != valMap.end() ) return cache->second;

    smatch what;
    if( regex_match( e, what, re3 )) {
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
    if ( regex_match( e, what, re2 )) {
        // not a
        const auto a = eval(what[1] );
        return ~a; // betwise not
    }
    // is a number or variable?
    const bool isNumber = regex_match( e, what, red );
    const auto result = isNumber ? stoi(e) : eval( varExp[e] );
    valMap[e] = result; // put in cache
    return result;
}

auto day07(const bool isFirstAnswer) {
    ifstream f("07/input.txt");
    
    regex re("(.+) -> (\\w+)");
    smatch what;

    string line;
    while (getline(f, line)) {
        if( regex_match( line, what, re )) {
            varExp[what[2]] = what[1];
        } else {
            cerr << "Failed to parse line: " << line;
        }
    }

    valMap.clear(); // init

    if ( !isFirstAnswer )
        varExp["b"] = "16076"; // answer 1

    return eval( varExp["a"] );
}

TEST_CASE( "Day07", "[07]" ) {
    REQUIRE(16076 == day07(true));
    REQUIRE(2797  == day07(false));
}
