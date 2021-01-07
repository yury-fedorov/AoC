package aoc16.d02;

import aoc16.common.IOUtil;
import org.junit.Test;
import com.google.common.collect.*; // TODO - can be replaced with ad-hoc solution
import java.util.*;
import org.javatuples.*;
import static org.junit.Assert.assertEquals;

public class Day02Test {

    final char [][] KEYPAD = { {'1','2','3'}, {'4','5','6'}, {'7','8','9'} };

    final char [][] KEYPAD2 = {
        { ' ', ' ', '1', ' ', ' ' },
        { ' ', '2', '3', '4', ' ' },
        { '5', '6', '7', '8', '9' },
        { ' ', 'A', 'B', 'C', ' ' },
        { ' ', ' ', 'D', ' ', ' ' }
    };

    enum Direction { UP, DOWN, LEFT, RIGHT }

    final static Map<Character,Direction> TO_DIR = ImmutableMap.of(
            'U', Direction.UP, 'D', Direction.DOWN, 'L', Direction.LEFT, 'R', Direction.RIGHT );

    static char at( char [][] keypad, Pair<Integer,Integer> p ) { return keypad[p.getValue1()][p.getValue0()]; }

    static Pair<Integer,Integer> xy( int x, int y ) { return Pair.with( x, y ); }

    static int bound( char [][] keypad, int v ) { return Math.min( Math.max( 0, v ), keypad.length -1 ); }

    static Pair<Integer,Integer> move( char [][] keypad, Pair<Integer, Integer> p, Direction d ) {
        final int dx = d == Direction.LEFT ? -1 : d == Direction.RIGHT ? 1 : 0;
        final int dy = d == Direction.UP   ? -1 : d == Direction.DOWN  ? 1 : 0;
        int x1 = bound( keypad, p.getValue0() + dx );
        int y1 = bound( keypad, p.getValue1() + dy );
        final var p1 = xy(x1,y1);
        return at( keypad, p1 ) == ' ' ? p : Pair.with( x1, y1 );
    }

    public static Pair<Integer,Integer> processLine( char [][] keypad, String input, Pair<Integer,Integer> p ) {
        for ( char d : input.toCharArray() ) {
            p = move( keypad, p, TO_DIR.get( d ) );
        }
        return p;
    }

    public static String answer( List<String> input, Pair<Integer,Integer> p, char [][] keypad ) {
        assertEquals( '5', at( keypad, p ) );
        var result = new StringBuilder();
        for ( var line : input ) {
            p = processLine( keypad, line, p );
            result.append( at( keypad, p) );
        }
        return result.toString();
    }

    @Test
    public void solution() {
        final var input = IOUtil.input( "d02" );
        var answer1 = answer( input, xy(1,1), KEYPAD );
        assertEquals( "answer 1", "38961", answer1 );
        var answer2 = answer( input, xy(0,2), KEYPAD2 );
        assertEquals( "answer 2", "46C92", answer2 );
    }
}