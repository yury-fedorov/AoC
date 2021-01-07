package aoc16.d07;

import aoc16.common.IOUtil;
import org.junit.Test;

import java.util.HashSet;
import java.util.Set;

import static org.junit.Assert.assertEquals;

public class Day07Test {

    boolean isAbba( CharSequence s ) {
        for ( int i = 0; i < s.length() - 3; i++ ) {
            final char ch0 = s.charAt(i);
            final char ch1 = s.charAt(i+1);
            if ( ch0 == s.charAt(i+3) && ch1 == s.charAt(i+2) && ch0 != ch1 ) return true;
        }
        return false;
    }

    Set<String> getAbaList( CharSequence s ) {
        final var result = new HashSet<String>();
        for ( int i = 0; i < s.length() - 2; i++ ) {
            final char ch0 = s.charAt(i);
            final char ch1 = s.charAt(i+1);
            if ( ch0 == s.charAt(i+2) && ch0 != ch1 ) result.add( new String( new char[] { ch0, ch1, ch0 } ) );
        }
        return result;
    }

    String inverseAba( String aba ) {
        final char ch0 = aba.charAt(0);
        final char ch1 = aba.charAt(1);
        return new String( new char[] { ch1, ch0, ch1 } );
    }

    @Test
    public void solution()  {
        final var input = IOUtil.input("d07");
        int answer1 = 0;
        int answer2 = 0;
        for ( final var l : input ) {
            final var s = new StringBuilder();

            final var seqOutside = new HashSet<String>();
            final var seqInside = new HashSet<String>();

            boolean hasOutside = false;
            boolean hasInside = false;
            for ( final char ch : ( l + '[' ).toCharArray() ) {
                if ( ch == '[' ) {
                    // just finished outside brackets sequence
                    hasOutside |= isAbba( s );
                    seqOutside.addAll( getAbaList( s ) );
                    s.setLength(0);
                } else if ( ch == ']' ) {
                    // just finished hypernet sequence (within square brackets)
                    hasInside |= isAbba(s);
                    seqInside.addAll( getAbaList( s ) );
                    s.setLength(0);
                } else {
                    s.append(ch);
                }
            }
            answer1 += ( hasOutside && !hasInside ) ? 1 : 0;
            for ( final var outAba : seqOutside ) {
                if ( seqInside.contains( inverseAba( outAba ) ) ) {
                    answer2++;
                    break;
                }
            }
        }
        assertEquals( "answer 1", 115, answer1 );
        assertEquals( "answer 2", 231, answer2 );
    }
}
