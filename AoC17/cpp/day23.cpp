#include <iostream>
#include <catch2/catch.hpp>

int solution( bool is_answer1 ) {
	auto n = 0; // part1

	auto a = is_answer1 ?  0 : 1;
	auto b = 0;
	auto c = 0;
	auto d = 0;
	auto e = 0;
	auto f = 0;
	auto g = 0;
	auto h = 0;

	b = 67;					// set b 67
	c = b;					// set c b
	if (a) goto l0;			// jnz a 2
	goto l1;				// jnz 1 5
l0: b *= 100; n++; 	        // mul b 100
	b += 100000;			// sub b -100000
	c = b;					// set c b
	c += 17000;				// sub c -17000
l1: l8:	f = 1;				// set f 1
	d = 2;					// set d 2
l4:	e = 2;					// set e 2

if ( is_answer1 ) {
    //  original slow code
    l3:	g = d;					// set g d
    	g *= e; n++;			// mul g e
    	g -= b;					// sub g b
    	if (g) goto l2;			// jnz g 2
    	f = 0;					// set f 0
    l2:	e++;					// sub e -1
    	g = e;					// set g e
    	g -= b;					// sub g b
    	if (g) goto l3;			// jnz g -8
}

/*  first phase of transformation  of the slow code
	while (true) {
		if (d * e == b) { f = 0; }
		e++;
		if (e == b) break;
	}
*/
	if ( !is_answer1 ) {
	    // optimized replacement of the commented block
		const auto e0 = e;
		const auto e1 = b;
		const auto bd = std::div(b, d);
		if (bd.rem == 0 && bd.quot >= e0 && bd.quot <= e1) f = 0;
		e = b;
	}

	d++;					// sub d -1
	g = d;					// set g d
	g -= b;					// sub g b
	if (g) goto l4;			// jnz g -13
	if (f) goto l5;			// jnz f 2
	h++;					// sub h -1
l5:	g = b;					// set g b
	g -= c;					// sub g c
	if (g) goto l6;			// jnz g 2
	goto l7;				// jnz 1 3
l6:	b += 17;				// sub b -17
	goto l8;				// jnz 1 -23
l7:
    return is_answer1 ? n : h;
}

TEST_CASE( "Day23", "[23]" )
{
    const auto a1 = solution(true);
    REQUIRE( a1 == 4225 );

    const auto a2 = solution(false);
    REQUIRE(a2 == 905);
}
