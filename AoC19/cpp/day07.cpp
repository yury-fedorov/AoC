#include "common.h"
#include "intcode_computer.h"

using namespace std;
using namespace intcode_computer;

namespace day07 {

    pair<Number,bool> single_run(const auto & code, Number setting, Number input) {
        Memory memory = code;
        Queue in;
        in.push(setting);
        in.push(input);
        Queue out;
        const auto is_halt = run(memory, in, out);
        return { out.back(), is_halt};
    }

    Number all_runs( const auto & code, span<Number> settings, const bool is_task_1 ) {
        Number input = 0;
        Number max_input = -1;
        bool is_halt = false;
        do {
            for ( const auto setting : settings ) {
                auto [output, is_halt] = single_run( code, setting, input );
                input = output;
            }
            max_input = max( input, max_input );
        } while ( !is_task_1 && !is_halt ); // task 1 once or task 2 till halt
        return is_task_1 ? input : max_input;
    }

    int solution( const auto&  code, vector<int> settings, bool is_task_1 ) {
        int result = -1;
        do {
            const auto r = all_runs(code, settings, is_task_1);
            result = max( result, r );
        } while ( std::next_permutation( settings.begin(), settings.end() ) );
        return result;
    }

    int answer1( const auto & code ) {
        auto settings = {0,1,2,3,4};
        return solution(code, settings, true);
    }

    int answer2( const auto & code) {
        auto settings = {5,6,7,8,9};
        return solution(code, settings, false);
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