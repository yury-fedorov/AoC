package aoc19;

import org.junit.Test;

import java.util.HashSet;
import java.util.List;
import java.util.stream.Collectors;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class Day10Test {

    public static record Point (int x, int y) {}
    public static final List<Integer> PRIMES = List.of(2,3,5,7,11,13,17,19,23);
    public static final char ASTEROID = '#';

    static int common_denominator( int a, int b ) {
        final var aa = Math.abs(a);
        final var ab = Math.abs(b);
        var mi = Math.min( aa, ab );
        var ma = Math.max( aa, ab );
        if ( mi == 0 ) return ma;
        if ( mi == 1 ) return 1;
        final var mi0 = mi;
        final var filterPrimes = PRIMES.stream().filter( p -> p <= mi0 ).collect( Collectors.toList());
        for ( int prime : filterPrimes ) {
            while (true) {
                if ( ( mi % prime ) != 0 ) break;
                if ( ( ma % prime ) != 0 ) break;
                mi = mi / prime;
                ma = ma / prime;
            }
        }

        assertTrue(mi0 > 0 &&  mi > 0);
        return ( mi0 == mi ) ? 1 : ( mi0 / mi );
    }

    static int countSeen( HashSet<Point> asteroids, Point from ) {
        final var passed = new HashSet<Point>();
        for ( var p : asteroids ) {
            int dx = p.x() - from.x();
            int dy = p.y() - from.y();
            final int cd = common_denominator( dx, dy );
            if ( cd == 0 ) continue;
            dx /= cd;
            dy /= cd;
            passed.add( new Point( dx, dy ) );
        }
        return passed.size();
    }

    static HashSet<Point> loadMap( String data ) {
        final var map = IOUtil.input(data);
        final var allPositions = new HashSet<Point>();
        for ( int y = 0; y < map.size(); y++ ) {
            for ( int x = 0; x < map.get(0).length(); x++ ) {
                final char at = map.get(y).charAt(x);
                if (at == ASTEROID) allPositions.add( new Point(x,y) );
            }
        }
        return allPositions;
    }

    static int answer1( HashSet<Point> map ) {
        return map.stream().mapToInt( p -> countSeen(map,p) ).max().orElse(0);
    }

    @Test
    public void test() {
        final var allPositions = loadMap("day10-sample");
        assertEquals( "answer 1", 8, answer1(allPositions));
    }

    @Test
    public void solution() {
        final var allPositions = loadMap("day10");
        assertEquals( "answer 1", 269, answer1(allPositions));
    }
}
