#include "common.h"
#include "intcode_computer.h"

using namespace std;
using namespace intcode_computer;

namespace day11 {

    enum class Color { Black = 0, White = 1 };
    enum class DirectionIndication { Left90Degrees = 0, Right90Degrees = 1 };
    enum class Direction { Up = 0, Right, Down, Left };
    using Point = pair<int,int>;
    using Panels = map<Point,Color>;

    Direction update_direction( Direction direction, DirectionIndication new_direction ) noexcept {
        int value = static_cast<int>(direction);
        value += new_direction == DirectionIndication::Right90Degrees ? 1 : -1;
        return static_cast<Direction>( value % 4 );
    }

    Point update_position( Point position, Direction direction ) noexcept {
        const auto [x,y] = position;
        switch(direction) {
            case Direction::Up:    return { x, y + 1 };
            case Direction::Right: return { x + 1, y };
            case Direction::Down:  return { x, y - 1 };
            case Direction::Left:  return { x - 1, y };
        }
        assert(false);
    }

    Number robot_run(const Memory & code) {
        Point position = {0,0};
        auto direction = Direction::Up; // start facing up
        Panels panels; // How many panels does it paint at least once?

        Memory memory = code;
        Queue in;
        in.push( static_cast<int>( Color::Black ) );
        Queue out;

        OnExecuted track = [&]( shared_ptr<Operation> o ){
            if ( o->command_ == Command::Out ) {
                // we are interested in last two output values: new color, new direction
                if ( out.size() % 2 == 1 ) {
                    // last is color
                    const Color new_color = static_cast<Color>( out.back() );
                    panels.insert( { position, new_color } );
                } else {
                    // last is direction
                    const auto new_direction = static_cast<DirectionIndication>( out.back() );
                    direction = update_direction( direction, new_direction );
                    position = update_position(position, direction);
                    const auto pp = panels.find(position);
                    const auto next_panel_color = pp != panels.end() ? pp->second : Color::Black;
                    in.push( static_cast<int>( next_panel_color ) );
                }
            }
        };

        run(memory, in, out, track);
        return panels.size();
    }

    Number answer1( const Memory & code ) {
        // TODO to debug SIGABRT: return robot_run(code);
        return code.size() ? 1 : 0;
    }

    Number answer2( const Memory & ) {
        return 0;
    }
}

TEST_CASE( "Day11", "[11]" ) {
    using namespace day11;

    ifstream f("../data/day11.txt");
    string line;
    getline(f, line);
    const auto data = load( line );

    SECTION( "11-1" ) {
        REQUIRE( answer1(data) == -1 );
    }

    SECTION( "11-2" ) {
        REQUIRE( answer2(data) == -2 );
    }
}
