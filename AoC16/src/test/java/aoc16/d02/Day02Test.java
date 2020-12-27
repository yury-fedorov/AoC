package aoc16.d02;

import org.junit.Test;

import com.google.common.collect.*;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;

import org.javatuples.*;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

public class Day02Test {

    final static int [][] KEYPAD = { {1,2,3}, {4,5,6}, {7,8,9} };

    enum Direction { UP, DOWN, LEFT, RIGHT }

    final static Map<Character,Direction> TO_DIR = ImmutableMap.of(
            'U', Direction.UP, 'D', Direction.DOWN, 'L', Direction.LEFT, 'R', Direction.RIGHT );

    static int at( Pair<Integer,Integer> p ) { return KEYPAD[p.getValue1()][p.getValue0()]; }

    static Pair<Integer,Integer> xy( int x, int y ) { return Pair.with( x, y ); }

    static int bound( int v ) { return Math.min( Math.max( 0, v ), KEYPAD.length -1 ); }

    static Pair<Integer,Integer> move( Pair<Integer, Integer> p, Direction d ) {
        final int dx = d == Direction.LEFT ? -1 : d == Direction.RIGHT ? 1 : 0;
        final int dy = d == Direction.UP   ? -1 : d == Direction.DOWN  ? 1 : 0;
        return Pair.with( bound( p.getValue0() + dx ), bound(p.getValue1() + dy ) );
    }

    public static Pair<Integer,Integer> processLine( String input, Pair<Integer,Integer> p ) {
        for ( char d : input.toCharArray() ) {
            p = move( p, TO_DIR.get( d ) );
        }
        return p;
    }

    @Test
    public void solution() throws IOException {
        final var url = getClass().getClassLoader().getResource( "d02/input.txt" );
        final var input = Files.readAllLines( Path.of(url.getPath()) );
        var p = xy(1,1);
        assertEquals( 5, at( p ) );
        var answer1 = new StringBuilder();
        for ( var line : input ) {
            p = processLine( line, p );
            answer1.append( at(p) );
        }
        assertEquals( "answer 1", "38961", answer1.toString() );
    }
}
