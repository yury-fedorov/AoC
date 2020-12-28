package aoc16.d03;

import org.junit.Test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;
import java.util.regex.Pattern;

import static org.junit.Assert.assertEquals;

public class Day04Test {

    static int validRoomSectorId( String line ) {
        final var patternString = "([a-z\\-]+)(\\d+)\\[([a-z]+)\\]$";
        final var pattern = Pattern.compile(patternString);
        final var matcher = pattern.matcher(line);
        while(matcher.find()) {
            final var p1 = matcher.group(1);
            final var p2 = Integer.parseInt( matcher.group(2) );
            final var p3 = matcher.group(3);
            final var frequency = new HashMap<Character,Integer>();
            for ( final var ch : p1.toCharArray() ) {
                final var counter = frequency.getOrDefault( ch, 0 );
                frequency.put(ch, counter + 1 );
            }
            // TODO
            // System.out.println("found: " + p1 + " -> " + p2 +" -> " + p3 );
            return p2;
        }
        System.out.println(line);
        throw new IllegalArgumentException();
    }
    
    @Test
    public void samples() {
        // is a real room because the most common letters are a (5), b (3), and then a tie between x, y, and z, which are listed alphabetically.
        assertEquals("sample 1", 123, validRoomSectorId("aaaaa-bbb-z-y-x-123[abxyz]") );
        // is a real room because although the letters are all tied (1 of each), the first five are listed alphabetically.
        assertEquals("sample 2", 987, validRoomSectorId("a-b-c-d-e-f-g-h-987[abcde]") );
        assertEquals("sample 3", 404, validRoomSectorId("not-a-real-room-404[oarel]") );
        assertEquals("sample 4", 0,   validRoomSectorId("totally-real-room-200[decoy]") );
    }

    @Test
    public void solution() throws IOException {
        final var url = getClass().getClassLoader().getResource( "d04/input.txt" );
        final var input = Files.readAllLines( Path.of(url.getPath()) );
        int answer1 = 0;
        for ( final var l : input ) {
            answer1 += validRoomSectorId(l);
        }
        assertEquals("answer 1", -1, answer1 );
        // assertEquals("answer 2", 1921, answer2 );
    }
}
