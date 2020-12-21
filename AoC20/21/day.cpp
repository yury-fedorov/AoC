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
#include <iterator>

using namespace std;

typedef string Food;
typedef set<Food> Foods;
typedef string Allergen;
typedef set<Allergen> Allergens;
typedef vector< pair< Foods, Allergens > > FoodAllergenList;

set<string> split( const string & list, const regex & re ) {
    set<string> result;
    smatch res;
    string::const_iterator searchStart( list.cbegin() );
    while ( regex_search( searchStart, list.cend(), res, re ) )
    {
        result.insert( res[1] );
        searchStart = res.suffix().first;
    }
    return result;
}

inline void addAll( set<string> & s, const set<string> & toBeAdded ) {
    for ( const auto & i : toBeAdded ) s.insert(i);
}

set<string> intersect( const set<string> & a, const set<string> & b ) {
    set<string> result;
    for ( const auto & ai : a ) {
        if ( b.find( ai ) != b.end() ) result.insert( ai );
    }
    return result;
}

int main() {
    FoodAllergenList list;

    const bool isFirstAnswer = true;

    ifstream f("input.txt");
    const regex re("^([\\w ]+) \\(contains (.+)\\)$");
    const regex reItem("([\\w]+)");
    string line;
    smatch what;
    while (getline(f, line)) {
        if( regex_match( line, what, re )) {
            const auto foods = what[1];
            const auto allergens = what[2];
            list.emplace_back( split( foods, reItem ), split( allergens, reItem ) );
        } else {
            cerr << "Unexpected line: " << line << endl;
        }
    }

    Foods allFoods;
    Allergens allAllergens;
    for ( const auto & [ foods, allergens ] : list ) {
        addAll( allAllergens, allergens );
        addAll( allFoods, foods );
    }

    cout << list.size() << " "<< allFoods.size() << " " << allAllergens.size() << endl; // 45 facts

    map<Allergen,Foods> allergenOptions;
    for ( const auto & a : allAllergens ) {
        cout << a << endl;
        Foods & optFoods = allergenOptions[a];
        addAll( optFoods, allFoods );
        for ( const auto [ foods, allergens ] : list ) {
            const bool containsAllergen = allergens.find( a ) != allergens.end();
            if ( containsAllergen ) {
                optFoods = intersect( optFoods, foods );
                if ( optFoods.size() == 1 ) {
                    const string & f = *optFoods.cbegin();
                    cout << "Found " << a << " -> " << f << endl;
                    break;
                }
            }
        }
        cout << allergenOptions[a].size() << endl;
    }

    // now we need to clean from found options
    while ( true ) {
        bool toRepeat = false;
        for ( const auto & [ allergen, foods ] : allergenOptions ) {
            if ( foods.size() == 1 ) {
                // found found
                const Food & f = *foods.cbegin();
                for ( auto & [ allergen1, foods1 ] : allergenOptions ) {
                    if ( allergen != allergen1 ) { 
                        toRepeat |= foods1.erase( f ) > 0;
                    }
                }
                if ( toRepeat ) break;
            }
        }
        if ( !toRepeat ) break;
    }

    Foods foodsWithoutAllergens { allFoods };
    Foods foodsWithAllergens;
    for ( const auto & [ allergen, foods ] : allergenOptions ) {
        const auto & f = *foods.cbegin();
        cout << allergen << " -> " << f << " " << foods.size() << endl;
        foodsWithAllergens.insert(f);
        foodsWithoutAllergens.erase(f);
    }

    int answer1 = 0;
    for ( const auto & [ foods, allergens ] : list ) {
        const auto && f = intersect( foods, foodsWithoutAllergens );
        answer1 += f.size();
    }

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? answer1 : 2 ) << endl;

    return 0;
}
