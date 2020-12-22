#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <regex>
#include <numeric>
#include <assert.h>
#include <climits>

using namespace std;

typedef int Card;
typedef list<int> Deck;
typedef vector<Deck> Decks;
typedef set<Decks> History;
enum Player { FIRST, SECOND };

Card next( Deck & d ) { const Card c = d.front(); d.pop_front(); return c; }

void put( Deck & d, Card c1, Card c2 ) { d.push_back( c1 ); d.push_back( c2 ); }

const bool isPrintOn = false;

void print( const Deck & d ) {
    if (!isPrintOn) return;
    cout << "Deck: ";
    for ( const Card c : d ) {
        cout << c << " ";
    }
    cout << endl;
}

Player recursiveCombat( Decks & decks ) {
    History history;

    Deck & d1 = decks[FIRST];
    Deck & d2 = decks[SECOND];

    while ( !d1.empty() && !d2.empty() ) {
        if (isPrintOn) cout << endl;
        print( d1 );
        print( d2 );

        const auto [pos, isNew] = history.insert( decks );
        if ( !isNew ) { 
            if (isPrintOn) cout << "Repeated position" << endl;
            return FIRST; 
        }
        Card c1 = next( d1 );
        Card c2 = next( d2 );
        if (isPrintOn) cout << "c1: " << c1 << " c2: " << c2 << endl;
        
        const bool d1l = d1.size() >= c1;
        const bool d2l = d2.size() >= c2;

        Player winner; 
        if ( d1l && d2l ) {
            Decks copy { decks };
            winner = recursiveCombat( copy );
            if (isPrintOn) cout << "Recursive combat: " << winner << endl;
        } else {
            winner = ( c1 > c2 ? FIRST : SECOND );
            if (isPrintOn) cout << "Classical more or less: " << winner << endl;
        }
        if ( winner == SECOND ) swap( c1, c2 ); // first card of the winner
        put( winner == FIRST ? d1 : d2, c1, c2 );
    }
    return d1.empty() ? SECOND : FIRST; 
}

long answer( const Deck & d ) {
    long result = 0;
    int n = d.size();
    for ( const Card c : d ) {
        result += n-- * c;
    }
    return result;
}

int main() {
    Decks decks;
    const bool isFirstAnswer = false;

    ifstream f("input.txt");
    const regex rePlayer("^Player (\\d+):$");
    string line;
    smatch what;
    while (getline(f, line)) {
        if( regex_match( line, what, rePlayer )) {
            const int playerN = stoi( what[1] );
            assert( playerN == 1 || playerN == 2 );

            Deck deck;
            while (getline(f, line) && !line.empty() ) {
                deck.push_back( stoi(line) );
            }
            decks.push_back(deck);
        } else {
            cerr << "Unexpected line: " << line << endl;
        }
    }
    Deck & d1 = decks[FIRST];
    Deck & d2 = decks[SECOND];
    Deck * d = nullptr;

    if ( isFirstAnswer ) {
        while ( !d1.empty() && !d2.empty() ) {
            const Card c1 = next( d1 );
            const Card c2 = next( d2 );
            Deck & d = c1 > c2 ? d1 : d2;
            const auto [ mi, ma ] = minmax( c1, c2 );
            assert( ma > mi );
            put( d, ma, mi );
        }
        d = d2.empty() ? &d1 : &d2;
    } else {
        d = recursiveCombat(decks) == FIRST ? &d1 : &d2;
    }
    assert( d != nullptr );
    const auto a = answer(*d);

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << a << endl;
    assert( a == ( isFirstAnswer ? 30138 : 0 ) );
    return 0;
}
