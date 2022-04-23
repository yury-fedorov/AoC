package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.concurrent.LinkedBlockingQueue;

import static org.junit.Assert.assertEquals;

public class Day13Test {

    record Point (long x, long y) {}
    final static long BLOCK = 2L;
    @Test
    public void solution() {
        final var  memory = IntcodeComputer.loadMemory(IOUtil.input("day13").get(0));
        final var in = new LinkedBlockingQueue<Long>();
        final var out = new LinkedBlockingQueue<Long>();
        IntcodeComputer.run( memory, in, out);
        final var a = new ArrayList<>(out);
        final var n = a.size() / 3;
        final var map = new HashMap<Point,Long>();
        for ( int i = 0; i < n; i++ ) {
            final var base = i * 3;
            var x = a.get( base );
            var y = a.get( base + 1 );
            var tileId = a.get( base + 2 );
            map.put( new Point(x,y), tileId );
        }
        assertEquals( "answer 1", 309L, map.values().stream().filter( t -> t == BLOCK ).count() );
        assertEquals( "answer 2", -2L, 0 );
    }
}
