#include <iostream>
#include <fstream>
#include <regex>
#include <assert.h>
#include <catch2/catch.hpp>

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

auto day12( const bool isFirstAnswer ) {
    Direction direction = dEast;
    int wpEast = 10;
    int wpNorth = 1;

    int east = 0;
    int north = 0;

    ifstream f("12/input.txt");
    regex re("^(\\w)(\\d+)$");
    smatch what;
    string line;
    while (getline(f, line)) {
        if( regex_match( line, what, re )) {
            const string cmd = what[1];
            const int value = stoi(what[2]);
            const char command = cmd[0];
            assert(cmd.length() == 1);

            if ( isFirstAnswer ) {
                switch (command) {
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
            } else {
                switch (command) {
                    case North: wpNorth += value; break;
                    case South: wpNorth -= value; break;
                    case East: wpEast += value; break;
                    case West: wpEast -= value; break;

                    case Forward: 
                    {
                        east += ( value * wpEast );
                        north += ( value * wpNorth );
                        break;
                    }
                        
                    case Left:  
                    case Right:
                    {
                        if ( value == 180 ) {
                            // just changing sign
                            wpEast *= -1;
                            wpNorth *= -1;
                        } else if ( value == 90 || value == 270 ) {
                            bool isLeft = command == Left;
                            if ( value == 270 ) {
                                // now we rotate only to 90 degrees
                                isLeft = !isLeft;
                            }
                            // we were going to east ( if now left -> nord (positive), right - south (negative) )
                            // we were going to north (positive) ( if now left -> west (negative), right -> east (positive) )
                            const int ke = isLeft ? -1 : 1;
                            const int kn = isLeft ? 1 : -1;

                            // hand made swap
                            const auto tmp = wpEast;
                            wpEast = wpNorth;
                            wpNorth = tmp;

                            wpEast *= ke;
                            wpNorth *= kn;
                        } else assert( false );
                        break;
                    }
                    default: assert(false);
                }                
            }
        } else {
            cerr << "Unexpected line: " << line << endl;
            FAIL();
        }
    }
    return ( abs(east) + abs(north) );
}

TEST_CASE( "Day12", "[12]" ) {
    REQUIRE( 923 == day12(true) );
    REQUIRE( 24769 == day12(false) );
}
