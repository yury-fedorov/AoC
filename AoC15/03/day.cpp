#include <iostream>
#include <set>
#include <fstream>
#include <sstream>

using namespace std;

int main() {
ifstream f("input.txt");
    stringstream buffer;
    buffer << f.rdbuf();
    const auto s = buffer.str();
    
    int x = 0;
    int y = 0;

    set< pair<int,int> > houses;
    houses.insert( {x,y} );
    for ( char d : s ) {
        switch(d) {
            case '^': y--; break;
            case 'v': y++; break;
            case '>': x++; break;
            case '<': x--; break;
            default: cerr << "Unexpected direction: " << d << endl;
        }
        houses.insert( {x,y} );
    }

    // Question 1
    cout << "Answer 1: " << houses.size() << endl;

    x = 0;
    y = 0;
    int a = 0;
    int b = 0;
    houses.clear();
    bool isSanta = true;
    for ( char d : s ) {
        int & _x = isSanta ? x : a;
        int & _y = isSanta ? y : b;
        switch(d) {
            case '^': _y--; break;
            case 'v': _y++; break;
            case '>': _x++; break;
            case '<': _x--; break;
            default: cerr << "Unexpected direction: " << d << endl;
        }
        houses.insert( {_x,_y} );
        isSanta = !isSanta;
    }

    // Question 2
    cout << "Answer 2: " << houses.size() << endl;

    return 0;
}
