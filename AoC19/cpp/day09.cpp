#include "common.h"
#include "intcode_computer.h"

using namespace std;
using namespace intcode_computer;

namespace day09 {

    Number single_run(const auto & code, Number input) {
        Memory memory = code;
        Queue in;
        in.push(input);
        Queue out;
        run(memory, in, out);
        return out.back();
    }

    Number answer1( const auto & code ) {
        // TODO to debug: return single_run(code, 1);
        return code.size() ? 1 : 0;
    }

    Number answer2( const auto & ) {
        return 0;
    }
}

TEST_CASE( "Day09", "[09]" ) {
    using namespace day09;

    ifstream f("../data/day09.txt");
    string line;
    getline(f, line);
    const Memory data = load( line );

    SECTION( "09-1" ) {
        REQUIRE( answer1(data) == -1 );
    }

    SECTION( "09-2" ) {
        REQUIRE( answer2(data) == -2 );
    }
}