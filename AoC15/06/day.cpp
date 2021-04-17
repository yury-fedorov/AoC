#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>
#include <regex>
#include <catch2/catch.hpp>

using namespace std;

struct Instruction {
    string operation;
    int x0;
    int y0;
    int x1;
    int y1;
};

auto solution(const bool isFirstAnswer) {
    ifstream f("06/input.txt");
    
    regex re("(.+) (\\d+),(\\d+) through (\\d+),(\\d+)");
    smatch what;

    vector<Instruction> list;

    string line;
    while (getline(f, line)) {
        if( regex_match( line, what, re )) {
            // cout << "Original: " << line << endl;
            // cout << what[1] << " " << what[2] << "," << what[3] << " - " << what[4] << "," << what[5] << endl;
            Instruction i;
            i.operation = what[1];
            i.x0 = stoi(what[2]);
            i.y0 = stoi(what[3]);
            i.x1 = stoi(what[4]);
            i.y1 = stoi(what[5]);
            list.push_back(i);
        } else {
            cerr << "Failed to parse line: " << line;
        }
    }

    // initialization of grid
    const int SIZE = 1000;
    int grid[SIZE][SIZE];
    for ( auto x = 0; x < SIZE; x++ )
        for ( auto y = 0; y < SIZE; y++ )
            grid[x][y] = 0;

    for ( const auto & i : list ) {
        for ( auto x = i.x0; x <= i.x1; x++ )
            for ( auto y = i.y0; y <= i.y1; y++ ) {
                auto & v = grid[x][y];

                if ( isFirstAnswer ) {
                    // executing instructions for answer 1
                    if ( i.operation == "turn on" ) grid[x][y] = 1;
                    else if (i.operation == "turn off" ) grid[x][y] = 0;
                    else if (i.operation == "toggle") grid[x][y] = grid[x][y] > 0 ? 0 : 1;
                    else cerr << "Unexpected operation: " << i.operation << endl;
                } else {
                    // executing instructions for answer 2
                    if ( i.operation == "turn on" ) v++;
                    else if (i.operation == "turn off" ) v = max( v - 1, 0 );
                    else if (i.operation == "toggle") v += 2;
                    else cerr << "Unexpected operation: " << i.operation << endl;
                }
            }
    }

    auto countOn = 0;
    for ( auto x = 0; x < SIZE; x++ )
        for ( auto y = 0; y < SIZE; y++ )
            countOn += grid[x][y];

    return countOn;
}

TEST_CASE( "Day06", "[06]" ) {
    REQUIRE(569999 == solution(true));
    REQUIRE(17836115 == solution(false));
}