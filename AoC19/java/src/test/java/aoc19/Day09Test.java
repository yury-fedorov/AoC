package aoc19;

import aoc19.computer.IntcodeComputer;
import aoc19.computer.operation.NothingOnEvent;
import org.junit.Test;

import java.util.ArrayList;
import java.util.concurrent.LinkedBlockingQueue;

import static org.junit.Assert.assertEquals;

public class Day09Test {
    @Test
    public void solution() {
        final ArrayList<Long> memory = new ArrayList<>(IntcodeComputer.loadMemory(IOUtil.input("day09").get(0)));
        var in = new LinkedBlockingQueue<Long>();
        var out = new LinkedBlockingQueue<Long>();
        in.add(1l);
        IntcodeComputer.run( memory, in, out, new NothingOnEvent());
        assertEquals( "answer 1", -1l, out.remove().longValue() ); //1102 too low
    }
}
