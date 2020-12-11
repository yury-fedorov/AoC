#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

typedef vector<string> Field;
enum Place { Floor = '.', EmptySeat = 'L', OccupiedSeat = '#' };

pair<int,int> getSize( const Field & f ) {
    return make_pair( f[0].size(), f.size() );
}

bool isIn( const Field & f, int x, int y ) {
    const auto [xn, yn] = getSize(f);
    return ( x >= 0 && y >= 0 && x < xn && y < yn );
}

Place get( const Field & f, int x, int y ) {
    return isIn( f, x, y ) ? ( (Place)f[y][x] ) : Floor;   
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

int countOccupied2( const Field & f, int x, int y ) {
    auto count = 0;
    for ( int dx = -1; dx <= 1; dx++ ) {
        for ( int dy = -1; dy <= 1; dy++ ) {
            if ( dx == 0 && dy == 0 ) continue;
            for ( auto k = 1; true; k++ ) {
                const auto x1 = x + ( k * dx );
                const auto y1 = y + ( k * dy );
                if ( !isIn( f, x1, y1 ) ) break;
                const auto seat = get(f, x1, y1);
                if ( seat == Floor ) continue;
                count += seat == OccupiedSeat;
                break;
            }
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
    return a == b; 
}

void print( const Field & f ) {
    for ( auto l : f ) {
        cout << l << endl;
    }
    cout << endl;
}

int main() {

    const bool isFirstAnswer = true;

    Field field;
    
    ifstream f("input.txt");
    string line;
    while (getline(f, line)) {
        field.push_back(line);
    }

    const int maxOccupied = isFirstAnswer ? 4 : 5;
    auto prevTotal = totalOccupied(field);
    const auto [xn, yn] = getSize(field);
    for ( int i = 0; true; i++ ) {
        Field newField (field) ;
        for ( int x = 0; x < xn; x++ ) {
            for ( int y = 0; y < yn; y++ ) {
                auto seat = get( field, x, y );
                const auto occupied = isFirstAnswer ? countOccupied( field, x, y ) : countOccupied2( field, x, y );
                if ( seat == EmptySeat && ( occupied == 0 ) ) {
                    seat = OccupiedSeat;
                } else if ( seat == OccupiedSeat && ( occupied >= maxOccupied ) ) {
                    seat = EmptySeat;
                }
                set( newField, x, y, seat );
            }
        } 
        if ( isSame(field, newField) ) break;
        prevTotal = totalOccupied(newField);
        swap( field, newField );
    }

    cout << "Answer " << (isFirstAnswer ? 1 : 2 ) << ": " << prevTotal << endl; // answer 1: 2344, answer 2: 2076

    return 0;
}
