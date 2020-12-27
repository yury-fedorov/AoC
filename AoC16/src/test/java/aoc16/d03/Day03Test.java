package aoc16.d03;

import org.junit.Test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.Assert.assertEquals;

public class Day03Test {

    public static boolean isTriangle( int a, int b, int c ) {
        return ( ( a + b ) > c ) && ( ( a + c ) > b ) && ( ( b + c ) > a );
    }

    @Test
    public void solution() throws IOException {
        final var url = getClass().getClassLoader().getResource( "d03/input.txt" );
        final var input = Files.readAllLines( Path.of(url.getPath()) );
        int answer1 = 0;
        for ( final var l : input ) {
            final var a = l.split( "\\s+" );
            answer1 += isTriangle( Integer.parseInt(a[0]), Integer.parseInt(a[1]), Integer.parseInt(a[2]) ) ? 1 : 0;
        }
        assertEquals("", -1, answer1 );
    }
}
