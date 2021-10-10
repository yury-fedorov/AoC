package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.util.ArrayList;
import java.util.concurrent.LinkedBlockingQueue;

import static org.junit.Assert.assertEquals;

public class Day07Test {
    //  @Test
    public void solution() {
        final var memory = IntcodeComputer.loadMemory(IOUtil.input("day07").get(0));
        var out = new LinkedBlockingQueue<Long>();
        IntcodeComputer.run( memory, 1L, out);
        assertEquals( "answer 1", -1L, out.remove().longValue() ); //1102 too low
    }
}
