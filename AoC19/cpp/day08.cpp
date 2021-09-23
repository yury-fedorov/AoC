#include "common.h"

using namespace std;

namespace day08 {
    using LevelData = map<char,int>;

    LevelData count_on_level( const auto & image, int from, int count ) {
        LevelData result;
        const auto indexes = rv::iota( from, from +  count );
        auto chars = indexes | rv::transform( [&image](const int index){ return image[index]; } ) | r::to_vector;
        r::for_each( chars, [&result](const char ch) { result[ch]++; } );
        return result;
    }

    auto answer1(const auto & image ) {
        const auto width = 25;
        const auto height = 6;
        const auto count_on_one_level = width * height;
        const int count_level = image.size() / count_on_one_level;
        const auto level_id = rv::iota( 0, count_level ) | r::to_vector;
        using LevelDataPair = pair<int,LevelData>;
        const vector< LevelDataPair > data = level_id | rv::transform( [&image](const int level)
            { return LevelDataPair( level, count_on_level(image, level * count_on_one_level, count_on_one_level ) ); } )
            | r::to_vector;
        const auto min_zero = r::min_element(data,
            [](const LevelDataPair & a, const LevelDataPair & b){ return a.second.at('0') < b.second.at('0'); });
        const LevelData & m = min_zero->second;
        return m.at('1') * m.at('2');
    }

    int answer2(const auto &  ) {
        return  0;
    }
}

TEST_CASE( "Day08", "[08]" ) {
    using namespace day08;

    ifstream f("../data/day08.txt");
    string line;
    getline(f, line);

    SECTION( "08-1" ) {
        REQUIRE( answer1(line) == 1584 );
    }

    SECTION( "08-2" ) {
        REQUIRE( answer2(line) == -2 );
    }
}

