#include "common.h"
#include "intcode_computer.h"

using namespace std;
using namespace intcode_computer;

namespace day07 {

    Number single_run(const auto & code, Number setting, Number input) {
        Memory memory = code;
        Queue in;
        in.push(setting);
        in.push(input);
        Queue out;
        run(memory, in, out);
        return out.back();
    }

    Number all_runs( const auto & code, span<Number> settings ) {
        Number input = 0;
        for ( const auto setting : settings ) {
            input = single_run( code, setting, input );
        }
        return input;
    }

    int solution( const auto&  code, vector<int> settings ) {
        int result = -1;
        do {
            const auto r = all_runs(code, settings);
            result = max( result, r );
        } while ( std::next_permutation( settings.begin(), settings.end() ) );
        return result;
    }

    int answer1( const auto & code ) {
        auto settings = rv::iota( 0, 5 ) | r::to_vector;
        return solution(code, settings);
    }

    int answer2( const auto & code) {
        auto settings = {5,6,7,8,9};
        return solution(code, settings);
    }
}

TEST_CASE( "Day07", "[07]" ) {
    using namespace day07;

    ifstream f("../data/day07.txt");
    string line;
    getline(f, line);
    const Memory data = load( line );

    SECTION( "07-1" ) {
        REQUIRE( answer1(data) == 70597 );
    }

    SECTION( "07-2" ) {
        REQUIRE( answer2(data) == -2 );
    }
}