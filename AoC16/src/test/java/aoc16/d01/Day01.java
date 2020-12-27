package aoc16.d01;

import static org.junit.Assert.assertEquals;
import org.junit.Test;

import java.io.IOException;
import java.nio.file.*;
import java.util.Arrays;

public class Day01 {

    enum Direction { North, West, South, East };
    final Direction [] list = { Direction.North, Direction.West, Direction.South, Direction.East };
    @Test
    public void part1() throws IOException {
        final var input = Files.readString( Path.of("/Users/fedoy/projects/AoC/AoC16/src/test/java/aoc16/d01/input.txt") );
        final var c = input.split(", ");
        Direction d = Direction.North;
        int x = 0;
        int y = 0;
        for ( var cc : c ) {
            final boolean isLeft = cc.charAt(0) == 'L';
            final int i = Arrays.binarySearch( list, d );
            int newIndex = i + ( isLeft ? -1 : 1 );
            newIndex += list.length;
            newIndex %= list.length;
            final var nd = list[newIndex];
            final int dx = nd == Direction.East ? 1 : nd == Direction.West ? -1 : 0;
            final int dy = nd == Direction.North ? 1 : nd == Direction.South ? -1 : 0;
            final int delta = Integer.parseInt( cc.substring(1).trim() );
            x += ( dx * delta );
            y += ( dy * delta );
            d = nd;
        }
        assertEquals( "answer 1", 291, x + y );
    }
}