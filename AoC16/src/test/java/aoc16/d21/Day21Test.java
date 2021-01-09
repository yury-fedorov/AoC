package aoc16.d21;

import aoc16.common.IOUtil;
import org.junit.Assert;
import org.junit.Test;

public class Day21Test {

    // swap position 4 with position 0
    static StringBuilder swapPosition( CharSequence t, int a, int b ) {
        final var t1 = new StringBuilder(t);
        t1.setCharAt( a, t.charAt(b) );
        t1.setCharAt( b, t.charAt(a) );
        return t1;
    }

    // swap letter d with letter b
    static StringBuilder swapLetter( CharSequence t, Character a, Character b ) {
        final var s = new StringBuilder(t);
        final int ai = s.indexOf( a.toString() );
        final int bi = s.indexOf( b.toString() );
        return swapPosition( t, ai, bi );
    }

    @Test
    public void test() {
        Assert.assertEquals( "ebcda", swapPosition( "abcde", 0, 4 ).toString() );
        Assert.assertEquals( "edcba", swapLetter( "ebcda", 'b', 'd' ).toString() );
    }

    @Test
    public void solution() {
        final var operations = IOUtil.input( "d21" );
        final var input = "abcdefgh";
        Assert.fail( "no solution yet" );
    }
}
