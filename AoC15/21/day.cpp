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

    const bool isFirstAnswer = false;

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
    // 40 + 31 + 40 = 111 (right)

    cout << battle( bossStartHitPoints, bossDamage - playerArmor, playerStartHitPoints, playerDemage - bossArmor ) << endl;

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? 1 : 2 ) << endl;

    return 0;
}
