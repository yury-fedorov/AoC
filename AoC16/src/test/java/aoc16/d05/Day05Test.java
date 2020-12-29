package aoc16.d05;

import org.junit.Test;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import java.security.*;
import java.util.HashMap;
import java.util.Locale;

public class Day05Test {
    final MessageDigest messageDigest;

    public Day05Test() {
        try {
            messageDigest = MessageDigest.getInstance("MD5");
        } catch ( Exception e ) {
            throw new RuntimeException( "MD5 not found", e );
        }
    }

    String md5(String text) {
        messageDigest.update(text.getBytes());
        final var sb = new StringBuilder();
        for ( final var b : messageDigest.digest() ) {
            sb.append( String.format("%02X", b) );
        }
        return sb.toString();
    }

    long fiveZeros( String prefix, long min, long max ) {
        for ( ; min <= max; min++ ) {
            final var text = prefix + min;
            final var h = md5(text);
            if ( h.startsWith( "00000" ) ) return min;
        }
        return min;
    }

    // @Test // on macOS - slow: takes 329 secs on macOS
    // on Andrei's PC - takes 226.5 seconds
    public void solution() {
        var input = "cxdnnyjw";
        // abc
        assertTrue( "first 5 zeros", md5( "abc3231929" ).startsWith("00000") );
        // slow assertEquals( "test", 3231929, fiveZeros("abc", 0, 3232000) );
        final var max = Long.MAX_VALUE;
        long min = 0;
        final var sb = new StringBuilder();
        final var map = new HashMap<Integer,Character>();
        for ( ; map.size() < 8; min++ ) {
            min = fiveZeros( input, min, max );
            final var c = md5( input + min );
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
