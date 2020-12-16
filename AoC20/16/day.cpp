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
#include <optional>

using namespace std;

typedef vector<int> Ticket;
typedef vector<Ticket> Tickets;
typedef pair<int, int> Range;
typedef vector<Range> Rule;
typedef map<string, Rule> Rules;

smatch parse( const string & line, const regex & re ) {
    smatch what;
    if( regex_match( line, what, re )) {
        return what;
    } 
    cerr << "Unexpected line: " << line << endl;
    throw domain_error("Unexpected format");
}

vector<int> parseList( const string & line, const regex & re ) {
    vector<int> result;
    smatch res;
    string::const_iterator searchStart( line.cbegin() );
    while ( regex_search( searchStart, line.cend(), res, re ) ) {
        const auto n = stoi(res[1]);
        result.push_back( n );
        searchStart = res.suffix().first;
    }
    return result;
} 

int main() {
    const bool isFirstAnswer = true;

    Rules rules;

    ifstream f("input");
    string line;

    const regex reRule("^([a-z ]+): (\\d+)-(\\d+) or (\\d+)-(\\d+)");
    while (getline(f, line) && !line.empty() ) {
        auto && m = parse(line, reRule);
        const Rule rule = { { stoi(m[2]), stoi(m[3]) }, { stoi(m[4]), stoi(m[5]) } };
        rules.insert( make_pair( m[1], rule ) );
    }
    getline(f, line);
    assert(line == "your ticket:"); 
    const regex reTicket("(\\d+)");
    getline(f, line);
    const Ticket && myTicket = parseList(line, reTicket);
    
    getline(f, line);
    assert( line.empty() );

    getline(f, line);
    assert( line == "nearby tickets:" );
    Tickets tickets;
    while (getline(f, line) ) {
        const Ticket && ticket = parseList(line, reTicket);
        tickets.push_back( ticket );
    }

    long ticketScanningErrorRate {0};
    for ( const auto & t : tickets ) {
        for ( const auto & f : t ) {
            bool isGoodField = false;
            for ( const auto [name,ranges] : rules ) {
                for ( const auto & r : ranges ) {
                    if ( f >= r.first && f <= r.second ) {
                        isGoodField = true;
                        break;
                    }
                }
                if ( isGoodField ) break;
            }
            if ( !isGoodField ) {
                ticketScanningErrorRate += f;
            }
        }
    }

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? ticketScanningErrorRate : 2 ) << endl;
    assert(ticketScanningErrorRate == 24980);
    return 0;
}
