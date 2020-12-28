package aoc16.d03;

import org.junit.Test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;

import static org.junit.Assert.assertEquals;

public class Day04Test {

    static int validRoomSectorId( String line ) {
        return 0; // TODO  
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
        final var url = getClass().getClassLoader().getResource( "d03/input.txt" );
        final var input = Files.readAllLines( Path.of(url.getPath()) );
        int answer1 = 0;
        for ( final var l : input ) {
            answer1 += validRoomSectorId(l);
        }
        assertEquals("answer 1", -1, answer1 );
        // assertEquals("answer 2", 1921, answer2 );
    }
}
