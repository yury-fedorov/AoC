package aoc16.d21;

import aoc16.common.IOUtil;
import org.junit.Assert;
import org.junit.Test;

public class Day21Test {

    // swap position 4 with position 0
    static StringBuilder swap( CharSequence t, int a, int b ) {
        final var t1 = new StringBuilder( t );
        t1.setCharAt( a, t.charAt(b) );
        t1.setCharAt( b, t.charAt(a) );
        return t1;
    }

    @Test
    public void test() {
        Assert.assertEquals( "ebcda", swap( "abcde", 0, 4 ).toString() );
    }

    @Test
    public void solution() {
        final var operations = IOUtil.input( "d21" );
        final var input = "abcdefgh";
        Assert.fail( "no solution yet" );
    }
}
