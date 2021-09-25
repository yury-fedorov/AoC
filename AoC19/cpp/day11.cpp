#include "common.h"
#include "intcode_computer.h"

using namespace std;
using namespace intcode_computer;

namespace day11 {

    Number single_run(const Memory & code, Number input) {
        Memory memory = code;
        Queue in;
        in.push(input);
        Queue out;
        run(memory, in, out);
        return out.back();
    }

    Number answer1( const Memory & code ) {
        // TODO to debug:return single_run(code, 1);
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
