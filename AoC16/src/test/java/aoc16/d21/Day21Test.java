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

    // reverse positions 0 through 4
    static StringBuilder reversePosition( CharSequence t, int a, int b ) {
        final var result = new StringBuilder();
        for ( int i = 0; i < a; i++ ) result.append( t.charAt(i) );
        for ( int i = a; i <= b; i++ ) result.append( t.charAt(b-(i-a)) );
        for ( int i = b + 1; i < t.length(); i++ ) result.append( t.charAt(i));
        return result;
    }

    // rotate left 1 step
    static StringBuilder rotateLeft( CharSequence t, int s ) {
        final var result = new StringBuilder(t);
        while ( --s >= 0 ) {
            final char ch = result.charAt(0);
            result.deleteCharAt(0);
            result.append(ch);
        }
        return result;
    }

    static StringBuilder rotateRight( CharSequence t, int s ) {
        final int lastIndex = t.length() - 1;
        final var result = new StringBuilder(t);
        while ( --s >= 0 ) {
            final char ch = result.charAt(lastIndex);
            result.deleteCharAt(lastIndex);
            result.insert(0, ch );
        }
        return result;
    }

    // rotate based on position of letter b
    static StringBuilder rotateByPosition( CharSequence t, Character c ) {
        final var s = new StringBuilder(t);
        final int i = s.indexOf( c.toString() );
        return rotateRight( t, 1 + i + ( i >= 4 ? 1 : 0 ) );
    }

    @Test
    public void test() {
        Assert.assertEquals( "ebcda", swapPosition( "abcde", 0, 4 ).toString() );
        Assert.assertEquals( "edcba", swapLetter( "ebcda", 'b', 'd' ).toString() );
        Assert.assertEquals( "abcde", reversePosition( "edcba", 0, 4 ).toString() );
        Assert.assertEquals( "bcdea", rotateLeft( "abcde", 1).toString() );
        Assert.assertEquals( "deabc", rotateRight( "abcde", 2).toString() );
        Assert.assertEquals( "ecabd", rotateByPosition( "abdec", 'b').toString() );
        Assert.assertEquals( "decab", rotateByPosition( "ecabd", 'd').toString() );
    }

    @Test
    public void solution() {
        final var operations = IOUtil.input( "d21" );
        final var input = "abcdefgh";
        Assert.fail( "no solution yet" );
    }
}
