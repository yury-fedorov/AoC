#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

typedef vector<string> Field;
enum Place { Floor = '.', EmptySeat = 'L', OccupiedSeat = '#' };

pair<int,int> getSize( const Field & f ) {
    return make_pair( f[0].size(), f.size() );
}

Place get( const Field & f, int x, int y ) {
    const auto [xn, yn] = getSize(f);
    return ( x >= 0 && y >= 0 && x < xn && y < yn ) ? ( (Place)f[y][x] ) : Floor;   
}

inline void set( Field & f, int x, int y, Place value ) {
    f[y][x] = value;   
}

int countOccupied( const Field & f, int x, int y ) {
    auto count = 0;
    for ( int dx = -1; dx <= 1; dx++ ) {
        for ( int dy = -1; dy <= 1; dy++ ) {
            if ( dx == 0 && dy == 0 ) continue;
            const auto x1 = x + dx;
            const auto y1 = y + dy;
            count += get(f, x1, y1) == OccupiedSeat;
        }
    }
    return count;
}

int totalOccupied(const Field & f) {
    const auto [xn, yn] = getSize(f);
    auto count = 0;
    for ( int x = 0; x < xn; x++ ) {
        for ( int y = 0; y < yn; y++ ) {
            const auto seat = get( f, x, y );
            count += seat == OccupiedSeat;
        }
    }
    return count;
}

bool isSame(const Field & a, const Field & b ) {
    const auto [xn, yn] = getSize(a);
    for ( int x = 0; x < xn; x++ ) {
        if ( a[x] != b[x] ) return false;
    }
    return true;    
}

void print( const Field & f ) {
    for ( auto l : f ) {
        cout << l << endl;
    }
    cout << endl;
}

int main() {

    const bool isFirstAnswer = false;

    Field field;

    ifstream f("input.txt");
    // ifstream f("test.txt");

    string line;
    while (getline(f, line)) {
        field.push_back(line);
    }

    auto prevTotal = totalOccupied(field);
    const auto [xn, yn] = getSize(field);
    for ( int i = 0; i < 10000; i++ ) {
        print(field);
        Field newField (field) ;
        for ( int x = 0; x < xn; x++ ) {
            for ( int y = 0; y < yn; y++ ) {
                auto seat = get( field, x, y );
                const auto occupied = countOccupied( field, x, y );
                if ( seat == EmptySeat && ( occupied == 0 ) ) {
                    seat = OccupiedSeat;
                } else if ( seat == OccupiedSeat && ( occupied >= 4 ) ) {
                    seat = EmptySeat;
                }
                set( newField, x, y, seat );
            }
        }
        const auto newTotal = totalOccupied(newField);
        cout << "Iteration: " << i << " prev: " << prevTotal << " now: " << newTotal << endl; 
        if ( /* prevTotal == newTotal && */ isSame(field, newField) ) break;
        prevTotal = newTotal;
        swap( field, newField );
    }
    print(field);

    cout << "Answer 1: " << prevTotal << endl; // 2250 - not right

    return 0;
}
