#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>
#include <climits>
#include <assert.h>

using namespace std;

enum Actor { PLAYER, BOSS };
enum Spell { MAGIC_MISSILE, DRAIN, SHIELD, POISON, RECHARGE };

typedef map<Spell, int> Spells;
typedef vector<int> HitPoints;
typedef long long Int;

const Int MAX_MANA { 10'000 };

const map < Spell, tuple<int,int> > SPELL_ATTR = {
    { MAGIC_MISSILE, { 53, 1 } }, 
    { DRAIN, { 73, 1 } },
    { SHIELD, { 113, 6 } },
    { POISON, { 173, 6 } },
    { RECHARGE, { 229, 5 } } 
};

const vector<Spell> SPELLS = { MAGIC_MISSILE, DRAIN, SHIELD, POISON, RECHARGE };

inline Actor other( Actor actor ) { return actor == PLAYER ? BOSS : PLAYER; }

inline int mana( Spell s ) { return get<0>( SPELL_ATTR.at(s) ); }
inline int duration( Spell s ) { return get<1>( SPELL_ATTR.at(s) ); }

inline bool canSpell( Spell s, const Spells & spells, Int playerMana ) {
    return ( mana(s) <= playerMana ) && ( spells.find( s ) == spells.end() );
}

void actionSpells( Spells & spells, HitPoints & hitPoints, Int & playerMana, const bool toPrint ) {
    vector<Spell> null;
    for ( auto & si : spells ) {
        assert( si.second > 0 );

        switch( si.first ) {
            case SHIELD: break;
            case POISON: hitPoints[BOSS] -= 3; break;
            case RECHARGE: playerMana += 101; break;
            default: assert(false);
        }
        si.second -= 1;
        if (toPrint) cout << "Spell in action: " << si.first << " its timer is now " << si.second << endl;
        if ( si.second <= 0 ) null.push_back( si.first );
    }
    for ( const auto & s : null ) spells.erase(s); // erase all not needed anymore
}

void actionSpell( Spell s, Spells & spells, HitPoints & hitPoints, Int & playerMana, Int & spentMana ) {
    const int spellManaCost = mana(s);
    playerMana -= spellManaCost;
    spentMana += spellManaCost;
    const auto d = duration(s);
    if ( d > 1 ) { spells.emplace( s, d ); }
    else if ( s == DRAIN ) {  
        hitPoints[PLAYER] += 2;
        hitPoints[BOSS] -= 2;
    } else if ( s == MAGIC_MISSILE ) {
        hitPoints[BOSS] -= 4;
    } else assert(false);
}

int playerArmor( const Spells & spells ) {
    auto i = spells.find( SHIELD );
    return i == spells.end() ? 0 : ( i->second > 0 ) ? 7 : 0;
}

Int minManaToWinPlayer( const bool isFirst, HitPoints hitPoints, const int bossDamage, Int playerMana, Spells spells, Actor first, int depth ) {
    const bool toPrint = false;
    Int spentMana = 0;
    auto actor = first;
    auto enemy = other(actor);
    for ( ; true; swap( actor, enemy ) ) {
        if ( toPrint ) {
            cout << endl << "-- " << ( actor == PLAYER ? "Player" : "Boss" ) << " turn" << endl;
            cout << "Player has " << hitPoints[PLAYER] << " hit points, " << playerArmor(spells) << " armor, " 
                << playerMana << " mana " << endl;
            cout << "Boss has " << hitPoints[BOSS] << " hit points" << endl;
        }

        if ( !isFirst && ( actor == PLAYER ) ) {
            hitPoints[PLAYER] -= 1;
            if ( hitPoints[PLAYER] <= 0 ) return MAX_MANA;
        }

        actionSpells( spells, hitPoints, playerMana, toPrint );
        if ( hitPoints[BOSS] <= 0 ) return spentMana;

        if ( actor == BOSS ) {
            hitPoints[PLAYER] -= bossDamage - playerArmor( spells );
        } else {
            vector<Spell> appliable;
            for ( const Spell s : SPELLS ) {
                if ( canSpell( s, spells, playerMana ) ) appliable.push_back( s );
            }
            if ( appliable.empty() ) return MAX_MANA;
            Spell bestSpell;
            if ( appliable.size() == 1 ) {
                bestSpell = *appliable.begin();
            } else if ( depth > 6 ) { // let's not go too deep for optimization of step (all effects are seen on this level)
                bestSpell = *appliable.begin(); // first appliable is cheapest
            } else {
                // simulations
                map<Spell, Int> options;
                for ( const Spell s : appliable ) {
                    HitPoints hitPoints1 { hitPoints };
                    Spells spells1 { spells };
                    Int playerMana1 { playerMana };
                    Int spentMana1 = 0;
                    actionSpell( s, spells1, hitPoints1, playerMana1, spentMana1 );
                    const auto m = spentMana1 + minManaToWinPlayer( isFirst, hitPoints1, bossDamage, playerMana1, spells1, BOSS, depth + 1 );
                    if ( m < MAX_MANA ) {
                        options.emplace( s, m );
                        if (toPrint) cout << "option " << s << " mana " << m << endl;
                    }
                }
                if ( options.empty() ) return MAX_MANA; // loose because no mana
                // selection of the best option
                const auto best = min_element( options.cbegin(), options.cend(), 
                    []( const pair<Spell,int> & a, const pair<Spell,int> & b ) {
                        return a.second < b.second;
                    } );
                if ( best->second >= MAX_MANA ) return MAX_MANA; // loose anyway
                bestSpell = best->first;
            }
            actionSpell( bestSpell, spells, hitPoints, playerMana, spentMana );
            if(toPrint) cout << "Player casts " << bestSpell << " mana " << playerMana << " spent mana " << spentMana << endl;
        }
        if ( hitPoints[enemy] <= 0 ) return actor == PLAYER ? spentMana : MAX_MANA;
    }
}

int main() {
    // cout << minManaToWinPlayer( {10, 13}, 8, 250, Spells(), PLAYER, 0 ) << endl << endl; // sample 1
    // cout << minManaToWinPlayer( {10, 14}, 8, 250, Spells(), PLAYER, 0 ) << endl << endl; // sample 2
    const int bossStartHitPoints = 58;
    const int playerStartHitPoints = 50;
    const int mana = 500;
    const int bossDamage = 9;
    HitPoints hitPoints { playerStartHitPoints , bossStartHitPoints };
    const auto answer1 = minManaToWinPlayer( true, hitPoints, bossDamage, mana, Spells(), PLAYER, 0 );
    cout << "answer 1: " << answer1 << endl;
    assert( answer1 == 1269 );
    const auto answer2 = minManaToWinPlayer( false, hitPoints, bossDamage, mana, Spells(), PLAYER, 0 );
    cout << "answer 2: " << answer2 << endl;
    assert( answer2 == 1309 );
    return 0;
}