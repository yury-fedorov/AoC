#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <regex>
#include <assert.h>
#include <catch2/catch.hpp>

using namespace std;

typedef map<int,string> Input;
typedef set<string> RuleSet;
typedef shared_ptr<RuleSet> RuleSetPtr;

RuleSetPtr append( const RuleSetPtr & a, const RuleSetPtr & b ) {
    RuleSetPtr result = make_shared<RuleSet>();
    for ( const auto & ea : *a ) {
        for ( const auto & eb : *b ) {
            result->emplace( ea + eb );
        }
    }
    return result;
}

RuleSetPtr rule( const Input & input, int ruleId );

RuleSetPtr append( const Input & input, int rule1, int rule2 ) {
    const auto && a1 = rule( input, rule1 );
    const auto && a2 = rule( input, rule2 );
    return append( a1, a2 );
}

void addAll( RuleSetPtr & a, const RuleSetPtr & b ) {
    for ( const auto & e : *b ) {
        a->emplace( e );
    }
}

static map<int, RuleSetPtr> cache;

RuleSetPtr rule( const Input & input, int ruleId ) {
    // to speed up bootstrapping
    const auto i = cache.find( ruleId );
    if ( i != cache.end() ) {
        return i->second;
    }

    const string & ruleText = input.at(ruleId);
    RuleSetPtr result = make_shared<RuleSet>();

    static const regex rePipe2("^(\\d+) (\\d+) \\| (\\d+) (\\d+)$");
    static const regex rePipe1("^(\\d+) \\| (\\d+)$");
    static const regex reLeaf("^\"(\\w)\"$");
    static const regex re2("^(\\d+) (\\d+)$");
    static const regex re1("^(\\d+)$");
    smatch what;
    if( regex_match( ruleText, what, rePipe2 )) {
        addAll( result, append( input, stoi(what[1]), stoi(what[2]) ) );
        addAll( result, append( input, stoi(what[3]), stoi(what[4]) ) );
    } else if( regex_match( ruleText, what, rePipe1 )) {
        addAll( result, rule(input, stoi(what[1]) ) );
        addAll( result, rule(input, stoi(what[2]) ) );
    } else if ( regex_match( ruleText, what, reLeaf ) ) {
        result->emplace( what[1] );
    } else if( regex_match( ruleText, what, re2 )) {
        return append( input, stoi(what[1]), stoi(what[2]) );
    } else if( regex_match( ruleText, what, re1 )) {
        return rule(input, stoi(what[1]) );
    } else {
        cerr << "Unexpected line: " << ruleText << endl;
        assert(false);
    }
    cache.emplace(ruleId, result);
    return result;
}

const int BLOCK_SIZE_31_42 = 8;
inline string block( const string & msg, int id ) {
    return msg.substr( id * BLOCK_SIZE_31_42, BLOCK_SIZE_31_42 );
}

inline bool matchBlock( const string & block, int ruleId ) {
    const auto & sp = cache.find( ruleId )->second;
    return sp->find( block ) != sp->end();
}

bool match( const string & msg, const vector<int> & rules ) {
    for ( int blockId = rules.size(); --blockId >= 0; ) {
        if ( !matchBlock( block(msg,blockId), rules[blockId] ) ) return false;
    }
    return true;
}

vector<int> rules( const int n, const int n11 ) {
    assert( n > 0 );
    assert( n11 > 0 );
    const int n8 = n - ( n11 * 2 );
    assert( n8 > 0 );
    const int n42 = n8 + n11;
    const int n31 = n11;
    vector<int> result( n42, 42 );
    for ( int i = 0; i < n31; i++ ) result.push_back( 31 );
    return result;
}

bool match( const string & msg ) {
    const int l = msg.length();
    // 0: 8 11
    // 8: 42 | 42 8 (it was just "8: 42")
    // 11: 42 31 | 42 11 31 ( it was "11: 42 31" )
    const auto dt = div( l, BLOCK_SIZE_31_42 );
    if ( dt.rem != 0 ) {
        cerr << "Bad length: " << l << " message: " << msg << endl;
        return false;
    }
    assert( dt.quot >= 3 );
    for ( int n11 = 1; ( n11 * 2 ) < dt.quot; n11++ ) {
        const int n31 = n11;
        const int n42 = dt.quot - n11;
        assert( n42 > n31 );
        const auto && r = rules( dt.quot, n11 );
        if ( match( msg, r ) ) return true;
    }
    return false;
}

auto day19(const bool isFirstAnswer) {
    Input input;
    vector<string> messages;

    ifstream f("19/input.txt");
    string line;

    const regex re("^(\\d+): (.+)$");
    smatch what;
    while ( getline(f, line) && !line.empty() ) {
        if( regex_match( line, what, re )) {
            input.emplace( stoi(what[1]), what[2] );
        } else {
            cerr << "Unexpected line: " << line << endl;
        }
    }

    while ( getline(f, line) ) {
        messages.push_back(line);
    }

    if ( isFirstAnswer ) {
        const auto && ruleSetPtr = rule(input, 0); // boostrapping
        const RuleSet & rules = *ruleSetPtr;
        // cout << "Rules: " << rules.size() << endl;

        int count = 0;
        for ( const auto & m : messages ) {
            const auto & i = rules.find( m );
            count += i != rules.end();
        }
        return count;
    } else {
        // 8: 42 | 42 8 (it was just "8: 42") 
        // 11: 42 31 | 42 11 31 ( it was "11: 42 31" )

        // partial boostrapping
        const auto & r42 = rule(input, 42);
        const auto & r31 = rule(input, 31);
        assert( r31->begin()->length() == BLOCK_SIZE_31_42 );
        assert( r42->begin()->length() == BLOCK_SIZE_31_42 );

        // proove that 31 and 42 does not have common elements
        set<string> r31n42 { *r31 };
        for ( const string & s : *r42 ) r31n42.emplace( s );
        assert( r31n42.size() == r31->size() + r42->size() );

        int answer2 = 0;
        for ( const auto & msg : messages ) {
            if ( match( msg ) ) {
                answer2++;
            }
        }
        return answer2;
    }
}

TEST_CASE( "Day19-SLOW", "[19]" ) {
    const auto runSlow = false; // takes 19 seconds
    if ( !runSlow ) return;
    REQUIRE( 291 == day19(true)  );
    REQUIRE( 409 == day19(false) ); 
}