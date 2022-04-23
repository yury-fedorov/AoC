package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.util.concurrent.LinkedBlockingQueue;

import static org.junit.Assert.assertEquals;

public class Day11Test {
    @Test
    public void solution() {
        final var  memory = IntcodeComputer.loadMemory(IOUtil.input("day11").get(0));
        var in = new LinkedBlockingQueue<Long>();
        var out = new LinkedBlockingQueue<Long>();
        IntcodeComputer.run( memory, in, out);
        assertEquals( "answer 1", -1L, 0);
        assertEquals( "answer 2", -2L, 0 );
    }
}
