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

enum Command {
    East = 'E',
    North = 'N',
    South = 'S',
    West = 'W',
    Forward = 'F',
    Left = 'L',
    Right = 'R'
};

enum Direction { dEast, dNorth, dWest, dSouth };

Direction rotate( bool toLeft, int value, Direction previousDirection ) {
    assert ( value % 90 == 0 );
    const int shift = (toLeft ? 1 : -1) * (value / 90);
    int shifted = (int)previousDirection + shift;
    if ( shifted < 0 ) shifted += 4;
    if ( shifted >= 4 ) shifted -= 4;
    return (Direction)( shifted );
}

int main() {

    assert( rotate( true, 90, dNorth ) == dWest );
    assert( rotate( false, 90, dEast ) == dSouth );
    assert( rotate( false, 180, dEast ) == dWest );
    assert( rotate( true, 180, dEast ) == dWest );

    const bool isFirstAnswer = false;

    Direction direction = dEast;
    int east = 0;
    int north = 0;

    ifstream f("input.txt");
    regex re("^(\\w)(\\d+)$");
    smatch what;
    string line;
    while (getline(f, line)) {
        if( regex_match( line, what, re )) {
            const string cmd = what[1];
            const int value = stoi(what[2]);
            assert(cmd.length() == 1);
            cout << cmd << " " << value << endl;
            switch ( cmd[0] ) {
                case North: north += value; break;
                case South: north -= value; break;
                case East: east += value; break;
                case West: east -= value; break;

                case Forward: {
                    int & dir = direction == dEast || direction == dWest ? east : north;
                    const bool isPlus = direction == dEast || direction == dNorth;
                    dir += ( isPlus ? 1 : -1 ) * value;
                    break;
                }
                    
                case Left:  direction = rotate( true,  value, direction ); break;
                case Right: direction = rotate( false, value, direction ); break;
                default: assert(false);
            }
            cout << direction << " " << east << " " << north << endl;
        } else {
            cerr << "Unexpected line: " << line << endl;
        }
    }

    cout << east << " " << north << " = " << ( abs(east) + abs(north) ) << endl;

    cout << "Answer " << ( isFirstAnswer ? 1 : 2 ) << ": " << ( isFirstAnswer ? 1 : 2 ) << endl;

    return 0;
}
