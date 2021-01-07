package aoc16.d05;

import aoc16.common.Config;
import aoc16.common.Md5Util;
import org.junit.Test;

import java.util.HashMap;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

// TODO - slow - can be applied pool of MD5 like in day 15
public class Day05Test {

    final static Md5Util util = new Md5Util();

    long fiveZeros( String prefix, long min, long max ) {
        for ( ; min <= max; min++ ) {
            final var text = prefix + min;
            final var h = util.md5(text);
            if ( h.startsWith( "00000" ) ) return min;
        }
        return min;
    }

    @Test
    // on macOS - slow: takes 329 secs on macOS
    // on Andrei's PC - takes 226.5 seconds
    // on ASUS 2011 - takes 287.7 seconds
    // on ASUS 2012 Windows 10 - 294.6 seconds
    public void solution() {
        if ( Config.isFast() ) return;
        var input = "cxdnnyjw";
        // abc
        assertTrue( "first 5 zeros", util.md5( "abc3231929" ).startsWith("00000") );
        // slow assertEquals( "test", 3231929, fiveZeros("abc", 0, 3232000) );
        final var max = Long.MAX_VALUE;
        long min = 0;
        final var sb = new StringBuilder();
        final var map = new HashMap<Integer,Character>();
        for ( ; map.size() < 8; min++ ) {
            min = fiveZeros( input, min, max );
            final var c = util.md5( input + min );
            final var p5 = c.charAt(5);
            final var p6 = c.charAt(6);
            if ( sb.length() < 8 ) {
                sb.append( p5 );
            }
            final int position = p5 - '0';
            if ( !map.containsKey( position ) ) {
                if ( position >= 0 && position < 8 ) {
                    map.put( position, p6 );
                }
            }
        }
        System.out.println();
        final var answer1 = sb.toString().toLowerCase();
        assertEquals( "answer 1", "f77a0e6e", answer1 );

        final var answer2 = new StringBuilder();
        for ( int i = 0; i < 8; i++ ) answer2.append( map.get(i) );

        assertEquals( "answer 2", "999828EC", answer2.toString() );
    }
}
