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

enum Actor { PLAYER, BOSS };

inline Actor other( Actor actor ) { return actor == PLAYER ? BOSS : PLAYER; }

bool battle( int bossHitPoints, int bossHit, int playerHitPoints, int playerHit ) {
    vector< pair< int, int > > pointsHit 
        = { {  playerHitPoints, playerHit }, { bossHitPoints, bossHit } };
    for ( Actor actor = PLAYER; true; ) {
        const Actor enemy = other(actor);
        pointsHit[enemy].first -= pointsHit[actor].second;
        if ( pointsHit[enemy].first <= 0 ) return actor == PLAYER;
        actor = enemy;
    }
}

int main() {

    const int bossStartHitPoints = 109;
    const int bossDamage = 8;
    const int bossArmor = 2;
    const int playerStartHitPoints = 100;

    /*
    const int approxDiff = (bossDamage - bossArmor) * bossStartHitPoints / playerStartHitPoints;
    cout << approxDiff << endl;
    */

    const int playerDemage = 7;
    const int playerArmor = 4;
    // 40 + 31 + 40 = 111 (right answer 1)

    cout << "Win? " << battle( bossStartHitPoints, bossDamage - playerArmor, playerStartHitPoints, playerDemage - bossArmor ) << endl;

    for ( int pd = 4; pd <= 11; pd++ ) {
        for ( int pa = 8; pa >= 0; pa-- ) {
            if ( !battle( bossStartHitPoints, bossDamage - pa, playerStartHitPoints, pd - bossArmor ) ) {
                cout << "pd = " << pd << " pa = " << pa << endl;
                break; // no sense go below in armor for this demage
            }
        }
    }
    // the run of combinations confirms that the only things that matters is the sum
    // of player's damage and armor that must be 10 to loose
    // 188 (right answer 2) - 8 cheapest weapon, 100 for damage ring +3, 80 for defence ring +3

    return 0;
}
