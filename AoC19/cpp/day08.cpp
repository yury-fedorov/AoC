#include "common.h"
#include <iostream>

using namespace std;

namespace day08 {
    using LevelData = map<char,int>;
    using Image = string;

    LevelData count_on_level( const Image & image, int from, int count ) {
        LevelData result;
        const auto indexes = rv::iota( from, from +  count );
        auto chars = indexes | rv::transform( [&image](const int index){ return image[index]; } ) | r::to_vector;
        r::for_each( chars, [&result](const char ch) { result[ch]++; } );
        return result;
    }

    constexpr int width = 25;
    constexpr int height = 6;
    constexpr int count_on_one_level = width * height;

    auto answer1(const Image & image ) {
        const int count_level = image.size() / count_on_one_level;
        const vector< LevelData > data = rv::iota( 0, count_level )
            | rv::transform( [&image](const int level)
                { return count_on_level(image, level * count_on_one_level, count_on_one_level ); } )
            | r::to_vector;
        const auto min_zero = r::min_element(data,
            [](const LevelData & a, const LevelData & b){ return a.at('0') < b.at('0'); });
        const LevelData & m = *min_zero;
        return m.at('1') * m.at('2');
    }

    char at( const Image & image, int index ) {
        constexpr char transparent = '2';
        constexpr char black = '0';
        constexpr char white = '1';
        const int count_level = image.size() / count_on_one_level;
        // The layers are rendered with the first layer in front and the last layer in back.
        const auto level_id = rv::iota( 0, count_level );
        for ( auto id : level_id )  {
            const auto global_index = id * count_on_one_level + index;
            const auto at_level = image[global_index];
            if ( at_level == transparent ) continue;
            assert( at_level == black || at_level == white );
            return at_level == black ? ' ' : '#';
        }
        return '?';
    }

    vector<string> answer2(const Image & image ) {
        const auto image_as_line = rv::iota( 0, count_on_one_level )
                | rv::transform( [&image](const int index) { return at( image, index); } )
                | r::to_vector;
        return rv::iota(0, height)
                | rv::transform( [&image_as_line](const  int line)
                    {
                        const auto i0 = image_as_line.begin() + ( line * width );
                        const auto i1 = i0 + width;
                        return string( i0, i1 );
                    } )
                | r::to_vector;
    }
}

TEST_CASE( "Day08", "[08]" ) {
    using namespace day08;

    ifstream f("../data/day08.txt");
    string image;
    getline(f, image);

    SECTION( "08-1" ) {
        REQUIRE( answer1(image) == 1584 );
    }

    SECTION( "08-2" ) {
        const auto a2 = answer2(image);
        r::for_each( a2, [](const auto &  l) { cout << l << endl; } ); // prints KCGEC
        REQUIRE( true );
    }
}