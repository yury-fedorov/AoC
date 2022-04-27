package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.util.concurrent.LinkedBlockingQueue;

import static aoc19.computer.AsciiUtil.out;
import static org.junit.Assert.assertEquals;

public class Day19Test {

    @Test
    public void solution() {

        final var N = 50;
        var answer1 = 0L;
        for ( var y = 0; y < N; y++ ) {
            for ( var x = 0; x < N; x++ ) {
                final var r = get( x, y );
                answer1 += r == 1 ? 1 : 0;
            }
        }

        assertEquals( "answer 1", 131, answer1);
        assertEquals( "answer 2", -2, 0 );
    }

    static long get( long x, long y ) {
        final var  memory = IntcodeComputer.loadMemory(IOUtil.input("day19").get(0));
        final var in = new LinkedBlockingQueue<Long>();
        final var out = new LinkedBlockingQueue<Long>();
        final var c = new IntcodeComputer(memory, in, out);
        c.in( x );
        c.in( y );
        var r = c.run();
        if ( c.outSize() == 0 )
            throw new RuntimeException( "Bad pair!" );
        return c.out();
    }
}
