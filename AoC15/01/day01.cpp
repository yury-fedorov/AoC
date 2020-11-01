#include <iostream>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

int main() {
    ifstream f("input.txt");
    stringstream buffer;
    buffer << f.rdbuf();
    const auto s = buffer.str();
    // const string s = "()())";
    
    auto floor = 0;
    auto position = 1;
    bool isPositionFound = false;
    for ( char next : s ) {
        switch ( next ) {
            case '(': floor++; break;
            case ')': floor--; break;
            default: cerr << "Unexpected symbol: " << next << endl; 
        }
        if (!isPositionFound){
            isPositionFound = (floor < 0);
            position += ( isPositionFound ? 0 : 1 );
        } 
    }
    // Question 1
    cout << "Final floor: " << floor << endl;

    // Question 2
    cout << "First time in basement: " << position << endl;

    return 0;
}
