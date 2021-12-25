#include <iostream>
#include <string>

// typedef std::stack<long> Input;
typedef std::string Input;
typedef long long Int;

inline void mul( Int & a, Int b ) {
    switch( b ) {
        case 0: a = 0; break;
        case 1: break;
        default: a /= b; break;
    }
}

Int run( Input & input ) {
    Int w = 0;
    Int x = 0;
    Int y = 0;
    Int z = 0;

    int index =  0;
    auto next = [&input, &index](){ // auto r = input.top(); input.pop(); return r; 
        return input[index++] - '0';
    };

    w = next(); // inp w
    // mul x 0
    // add x z
    // mod x 26
    // div z 1
    // x = 12; // add x 12
    // x = 0; // eql x w
    x = 1; // eql x 0
    // mul y 0
    // y = 25; // add y 25
    // mul y x
    // y++; // add y 1
    // mul z y
    // y = 0; // mul y 0
    y = w;  // add y w
    y += 7; // add y 7
    // mul y x
    z = y; // add z y
    w = next(); // inp w
    // x = 0; // mul x 0
    x = z; // add x z
    x %= 26; // mod x 26
    // div z 1
    x += 11; // add x 11
    x = x != w ? 1 : 0; // eql x w -- x above 9
                        // eql x 0
    // mul y 0
    y = 25; // add y 25
    // mul y x
    y++; // add y 1
    // z *= y; // mul z y
    mul( z, y );

    // mul y 0
    y = w; // add y w
    y += 15; // add y 15
    // mul y x -- last x is 1
    z += y; // add z y
    w = next(); // inp w
    // mul x 0
    x = z; // add x z
    if (x<0) return -1; x %= 26; // mod x 26
    // div z 1
    x +=12; // add x 12
    // x = x == w ? 1 : 0; // eql x w
    // x = x == 0 ? 1 : 0; // eql x 0
    x = x != w ? 1 : 0;
    // mul y 0
    y = 25; // add y 25
    //  y *= x; // mul y x
    mul( y, x );
    y++; // add y 1
    // z *= y; // mul z y
    mul(z, y);
    // mul y 0
    y = w; // add y w
    y += 2;// add y 2
    // y *= x; // mul y x
    mul( y, x);
    z += y; // add z y
    w = next(); // inp w
    // mul x 0
    x = z; // add x z

    if (x<0) return -1;
	x %= 26; // mod x 26

	z /= 26; // div z 26
    x -= 3; // add x -3
    // x = x == w ? 1 : 0; // eql x w
    // x = x == 0 ? 1 : 0; // eql x 0
    x = x != w ? 1 : 0;
    // mul y 0
    y = 25; // add y 25
    // y *= x; // mul y x
    mul(y,x);
    y++; // add y 1
    z *= y; // mul z y
    // mul y 0
    y = w; // add y w
    y += 15; // add y 15
    // y *= x; // mul y x
    mul(y,x);
    z += y; // add z y
    w = next(); // inp w
    // mul x 0
    x = z; // add x z
    if (x<0) return -1; x %= 26; // mod x 26
    // div z 1
    x += 10; // add x 10
    // x = x == w ? 1 : 0; // eql x w
    // x = x == 0 ? 1 : 0; // eql x 0
    x = x != w ? 1 : 0;
    // mul y 0
    y = 25; // add y 25
    // y *= x; // mul y x
    mul(y,x);
    y++; // add y 1
    // z *= y; // mul z y
    mul(z,y);
    // mul y 0
    y = w; // add y w
    y +=14; // add y 14
    // y *= x; // mul y x
    mul(y,x);
    z += y; // add z y
    w = next(); // inp w
    // mul x 0
    x = z; // add x z
    if (x<0) return -1; x %= 26; // mod x 26
    z /= 26; // div z 26
    x -= 9; // add x -9
    // x = x == w ? 1 : 0; // eql x w
    // x = x == 0 ? 1 : 0; // eql x 0
    x = x != w ? 1 : 0;
    // mul y 0
    y = 25; // add y 25
    // y *= x; // mul y x
    mul(y, x);
    y++; // add y 1
    // z *= y; // mul z y
    mul(z,y);
    // mul y 0
    y = w; // add y w
    y += 2; // add y 2
    // y *= x; // mul y x
    mul(y,x);
    z += y; // add z y
    w = next(); // inp w
    // mul x 0
    x = z; // add x z
    if (x<0) return -1; x %= 26; // mod x 26
    // div z 1
    x += 10; // add x 10
    // x = x == w ? 1 : 0; // eql x w
    // x = x == 0 ? 1 : 0; // eql x 0
    x = x != w ? 1 : 0;
    // mul y 0
    y = 25; // add y 25
    // y *= x; // mul y x
    mul(y,x);
    y++; // add y 1
    //  z *= y; // mul z y
    mul(z,y);
    // mul y 0
    y = w; // add y w
    y += 15; // add y 15
    //  y *= x; // mul y x
    mul(y, x);
    z += y; // add z y
    w = next(); // inp w
    // mul x 0
    x = z; // add x z
    if (x<0) return -1; x %= 26; // mod x 26
    z /= 26; // div z 26
    x -= 7; // add x -7
    // x = x == w ? 1 : 0; // eql x w
    // x = x == 0 ? 1 : 0; // eql x 0
    x = x != w ? 1 : 0;
    // mul y 0
    y = 25; // add y 25
    // y *= x; // mul y x
    mul( y, x);
    y++; // add y 1
    // z *= y; // mul z y
    mul(z,y);
    // mul y 0
    y = w; // add y w
    y++; // add y 1
    // y *= x; // mul y x
    mul(y, x);
    z += y; // add z y
    w = next(); // inp w
    // mul x 0
    x = z; // add x z
    if (x<0) return -1; x %= 26; // mod x 26
    z /= 26; // div z 26
    x -= 11; // add x -11
    // x = x == w ? 1 : 0; // eql x w
    // x = x == 0 ? 1 : 0; // eql x 0
    x = x != w ? 1 : 0;
    // mul y 0
    y = 25; // add y 25
    // y *= x; // mul y x
    mul(y,x);
    y++; // add y 1
    // z *= y; // mul z y
    mul(z,y);
    // mul y 0
    y = w; // add y w
    y += 15; // add y 15
    // y *= x; // mul y x
    mul(y,x);
    z += y; // add z y
    w = next(); // inp w
    // mul x 0
    x = z; // add x z
    if (x<0) return -1; x %= 26; // mod x 26
    z /= 26; // div z 26
    x -= 4; // add x -4
    // x = x == w ? 1 : 0; // eql x w
    // x = x == 0 ? 1 : 0; // eql x 0
    x = x != w ? 1 : 0;
    // mul y 0
    y = 25; // add y 25
    //  y *= x; // mul y x
    mul(y,x);
    y++; // add y 1
    // z *= y; // mul z y
    mul(z,y);
    // mul y 0
    y = w; // add y w
    y += 15; // add y 15
    // y *= x; // mul y x
    mul(y,x);
    z += y; // add z y
    w = next(); // inp w
    // mul x 0
    x = z; // add x z
    if (x<0) return -1; x %= 26; // mod x 26
    // div z 1
    x += 14; // add x 14
    // x = x == w ? 1 : 0; // eql x w
    // x = x == 0 ? 1 : 0; // eql x 0
    x = x != w ? 1 : 0;
    // mul y 0
    y = 25; // add y 25
    // y *= x; // mul y x
    mul(y,x);
    y++; // add y 1
    // z *= y; // mul z y
    mul(z,y);
    // mul y 0
    y = w; // add y w
    y += 12; // add y 12
    // y *= x; // mul y x
    mul(y,x);
    z += y; // add z y
    w = next(); // inp w
    // mul x 0
    x = z; // add x z
    if (x<0) return -1; x %= 26; // mod x 26
    // div z 1
    x += 11; // add x 11
    // x = x == w ? 1 : 0; // eql x w
    // x = x == 0 ? 1 : 0; // eql x 0
    x = x != w ? 1 : 0;
    // mul y 0
    y = 25; // add y 25
    // y *= x; // mul y x
    mul(y,x);
    y++; // add y 1
    // z *= y; // mul z y
    mul(z,y);
    // mul y 0
    y = w; // add y w
    y += 2; // add y 2
    // y *= x; // mul y x
    mul(y,x);
    z += y;// add z y
    w = next(); // inp w
    // mul x 0
    x = z; // add x z
    if (x<0) return -1; x %= 26; // mod x 26
    z /= 26; // div z 26
    x -= 8; // add x -8
    // x = x == w ? 1 : 0; // eql x w
    // x = x == 0 ? 1 : 0; // eql x 0
    x = x != w ? 1 : 0;
    // mul y 0
    y = 25; // add y 25
    // y *= x; // mul y x
    mul(y,x);
    y++; // add y 1
    // z *= y; // mul z y
    mul(z, y);
    // mul y 0
    y = w; // add y w
    y += 13; // add y 13
    // y *= x; // mul y x
    mul(y, x);
    z += y; // add z y

    w = next(); // inp w
    // mul x 0
    x = z; // add x z
    if (x<0) return -1; x %= 26; // mod x 26
    z /= 26; // div z 26
    x -= 10; // add x -10
    // x = x == w ? 1 : 0; // eql x w
    // x = x == 0 ? 1 : 0; // eql x 0
    x = x != w ? 1 : 0;
    // mul y 0
    y = 25; // add y 25
    // y *= x; // mul y x
    mul(y, x);
    y++; // add y 1
    // z *= y; // mul z y
    mul(z,y);
    // mul y 0
    y = w; // add y w
    y += 13; // add y 13
    // y *= x; // mul y x
    mul(y,x);
    z += y; // add z y
    return z;
}

int main() {
    const Int imax = 999'999'999'999'99L;
    const Int imin = 111'111'111'111'11L;
    for ( auto i = imax; i >= imin; i-- ) {  
        Input input = std::to_string(i);
        if ( input.find( '0' ) != std::string::npos ) continue;
        auto z = run( input );
        std::cout << i << "  "  << z << std::endl;
        if ( z == 0 ) {
            // 99999999999969 - wrong too high
            std::cout << "Found " << i << std::endl;
            return 0;
        }
        // std::cout << '.'; 
    }
    return 1;
}
