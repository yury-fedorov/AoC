#include "common.h"
#include "intcode_computer.h"

using namespace std;
using namespace intcode_computer;

namespace day09 {

    Number single_run(const auto & code, Number setting, Number input) {
        Memory memory = code;
        Queue in;
        in.push(setting);
        in.push(input);
        Queue out;
        run(memory, in, out);
        return out.back();
    }

    int answer1( const auto &  ) {
        return 0;
    }

    int answer2( const auto & ) {
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
