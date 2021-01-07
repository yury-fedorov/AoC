package aoc16.d16;

import org.junit.Assert;
import org.junit.Test;

public class Day16Test {

    static CharSequence next( CharSequence s ) {
        final var b = new StringBuilder( ( s.length() << 2 ) + 1 );
        b.append( s );
        b.append( '0' );
        for ( final char ch : new StringBuilder(s).reverse().toString().toCharArray() ) {
            b.append( ch == '1' ? '0' : '1' );
        }
        return b;
    }

    static CharSequence checksum( CharSequence s ) {
        final var l = s.length();
        var b = new StringBuilder( ( l >> 2 ) + 1 );
        for ( int i = 0; i < l; i += 2 ) {
            b.append( s.charAt( i ) == s.charAt( i + 1 ) ? '1' : '0' );
        }
        CharSequence result = b;
        while ( result.length() % 2 == 0 ) {
            result = checksum( result );
        }
        return result;
    }

    @Test
    public void test() {
        Assert.assertEquals( "100", next( "1" ).toString() );
        Assert.assertEquals( "001", next( "0" ).toString() );
        Assert.assertEquals( "11111000000", next( "11111" ).toString() );
        Assert.assertEquals( "1111000010100101011110000", next( "111100001010" ).toString() );
        Assert.assertEquals( "100", checksum("110010110100").toString() );
    }

    static String checksum( CharSequence input, final int diskLength ) {
        for ( ; input.length() < diskLength; input = next(input) ) {}
        final var disk = input.subSequence( 0, diskLength );
        Assert.assertEquals( diskLength, disk.length() );
        return checksum( disk ).toString();
    }

    @Test
    public void solution() {
        final var INPUT = "10010000000110000";
        Assert.assertEquals( "answer 1", "10010110010011110", checksum( INPUT, 272 ) );
        Assert.assertEquals( "answer 2", "01101011101100011", checksum( INPUT, 35651584 ) );
    }
}
