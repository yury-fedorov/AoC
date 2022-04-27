package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.util.Arrays;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;

import static aoc19.computer.AsciiUtil.out;
import static aoc19.computer.AsciiUtil.readOut;
import static org.junit.Assert.assertEquals;

public class Day17Test {

    @Test
    public void solution() {
        final var  memory = IntcodeComputer.loadMemory(IOUtil.input("day17").get(0));
        final var in = new LinkedBlockingQueue<Long>();
        final var out = new LinkedBlockingQueue<Long>();
        final var comp = new IntcodeComputer(memory, in, out);
        comp.run();
        final var mapAsString = readOut(out).toString();
        final var map = Arrays.stream(mapAsString.split( "\n" )).map( s -> s.trim() ).toList();
        final var maxX = map.get(0).length();
        final var maxY = map.size();
        long answer1 = 0;
        for ( var y = 0; y < maxY; y++ ) {
            for ( var x = 0; x < maxX; x++ ) {
                answer1 += isIntersection(map, x, y) ? x * y : 0;
            }
        }

        assertEquals( "answer 1", 4600, answer1);
        assertEquals( "answer 2", -2, 0 );
    }

    static char get( List<String> map, int x, int y ) {
        if ( y < 0 || y >= map.size() ) return OPEN_SPACE;
        final var line = map.get(y);
        if ( x < 0 || x >= line.length() ) return OPEN_SPACE;
        return line.charAt(x);
    }
    static final char SCAFFOLD = '#';
    static final char OPEN_SPACE = '.';
    static boolean isIntersection( List<String> map, int x, int y ) {
        if ( get( map, x, y ) != SCAFFOLD ) return false;
        if ( get( map, x + 1, y ) != SCAFFOLD ) return false;
        if ( get( map, x - 1, y ) != SCAFFOLD ) return false;
        if ( get( map, x, y + 1 ) != SCAFFOLD ) return false;
        return get( map, x, y - 1 ) == SCAFFOLD;
    }
}
