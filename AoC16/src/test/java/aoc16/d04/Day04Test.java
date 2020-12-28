package aoc16.d03;

import org.javatuples.Triplet;
import org.junit.Test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;
import java.util.regex.Pattern;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class Day04Test {

    static Triplet<String, Integer, String> parse( String line ) {
        final var patternString = "([a-z\\-]+)(\\d+)\\[([a-z]+)\\]$";
        final var pattern = Pattern.compile(patternString);
        final var matcher = pattern.matcher(line);
        while(matcher.find()) {
            final var p1 = matcher.group(1);
            final var p2 = Integer.parseInt( matcher.group(2) );
            final var p3 = matcher.group(3);
            return Triplet.with( p1, p2, p3 );
        }
        System.out.println(line);
        throw new IllegalArgumentException();
    }

    static int validRoomSectorId( Triplet<String,Integer,String> p ) {
        final var p1 = p.getValue0();
        final var p2 = p.getValue1();
        final var p3 = p.getValue2();
        final var frequency = new HashMap<Character,Integer>();
        for ( final var ch : p1.toCharArray() ) {
            if ( ch == '-' ) continue;
            final var counter = frequency.getOrDefault( ch, 0 );
            frequency.put(ch, counter + 1 );
        }
        int lastFrequency = p1.length() + 1;
        for ( final var ch : p3.toCharArray() ) {
            final var f = frequency.getOrDefault( ch, 0 );
            if ( ( f == 0 ) || ( f > lastFrequency ) ) return 0;
            frequency.remove(ch);
            lastFrequency = Math.min( f, lastFrequency );
        }
        for ( final var e : frequency.entrySet() ) {
            if ( e.getValue() > lastFrequency ) return 0;
        }
        return p2;
    }
    
    @Test
    public void samples() {

        // is a real room because the most common letters are a (5), b (3), and then a tie between x, y, and z, which are listed alphabetically.
        assertEquals("sample 1", 123, validRoomSectorId(parse( "aaaaa-bbb-z-y-x-123[abxyz]") ) );
        // is a real room because although the letters are all tied (1 of each), the first five are listed alphabetically.
        assertEquals("sample 2", 987, validRoomSectorId(parse( "a-b-c-d-e-f-g-h-987[abcde]") ) );
        assertEquals("sample 3", 404, validRoomSectorId(parse("not-a-real-room-404[oarel]") ) );
        assertEquals("sample 4", 0,   validRoomSectorId(parse("totally-real-room-200[decoy]") ) );
    }

    @Test
    public void solution() throws IOException {
        final var url = getClass().getClassLoader().getResource( "d04/input.txt" );
        final var input = Files.readAllLines( Path.of(url.getPath()) );
        long answer1 = 0;
        for ( final var l : input ) {
            final var p = parse(l);
            answer1 += validRoomSectorId(p);
        }
        assertEquals("answer 1", 278221, answer1 );
        // assertEquals("answer 2", 1921, answer2 );
    }
}
