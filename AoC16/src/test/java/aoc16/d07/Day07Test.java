package aoc16.d07;

import org.junit.Test;
import static org.junit.Assert.assertEquals;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

public class Day07Test {

    boolean isAbba( CharSequence s ) {
        for ( int i = 0; i < s.length() - 3; i++ ) {
            final char ch0 = s.charAt(i);
            final char ch1 = s.charAt(i+1);
            if ( ch0 == s.charAt(i+3) && ch1 == s.charAt(i+2) && ch0 != ch1 ) return true;
        }
        return false;
    }

    @Test
    public void solution() throws IOException {
        final var url = getClass().getClassLoader().getResource( "d07/input.txt" );
        final var input = Files.readAllLines( Path.of(url.getPath()) );
        int answer1 = 0;
        int answer2 = 0;
        for ( final var l : input ) {
            final var s = new StringBuilder();
            boolean hasOutside = false;
            boolean hasInside = false;
            for ( final char ch : ( l + '[' ).toCharArray() ) {
                if ( ch == '[' ) {
                    // just finished outside brackets sequence
                    hasOutside |= isAbba( s );
                    s.setLength(0);
                } else if ( ch == ']' ) {
                    // just finished hypernet sequence (within square brackets)
                    hasInside |= isAbba(s);
                    s.setLength(0);
                } else {
                    s.append(ch);
                }
            }
            answer1 += ( hasOutside && !hasInside ) ? 1 : 0;
        }
        assertEquals( "answer 1", 115, answer1 );
    }
}
