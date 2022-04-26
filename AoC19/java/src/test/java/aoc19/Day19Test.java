package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.util.concurrent.LinkedBlockingQueue;

import static aoc19.computer.AsciiUtil.out;
import static org.junit.Assert.assertEquals;

public class Day19Test {

    @Test
    public void solution() {
        final var  memory = IntcodeComputer.loadMemory(IOUtil.input("day19").get(0));
        final var in = new LinkedBlockingQueue<Long>();
        final var out = new LinkedBlockingQueue<Long>();
        final var comp = new IntcodeComputer(memory, in, out);
        comp.run();
        out(out);
        assertEquals( "answer 1", -1, 0);
        assertEquals( "answer 2", -2, 0 );
    }
}
