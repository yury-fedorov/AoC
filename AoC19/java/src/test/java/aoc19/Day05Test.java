package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.util.concurrent.LinkedBlockingQueue;

import static org.junit.Assert.assertEquals;

public class Day05Test {

    static int answer(String code, int in) {
        final var memory = IntcodeComputer.loadMemory(code);
        var out = new LinkedBlockingQueue<Long>();
        IntcodeComputer.run(memory, in, out);
        Long result = null;
        while (!out.isEmpty()) {
            result = out.remove();
        }
        return result.intValue();
    }

    @Test
    public void test() {
        final var read_write = "3,0,4,0,99";
        assertEquals("read write 1", 1, answer(read_write, 1));
        assertEquals("read write 2", 2, answer(read_write, 2));
    }

    @Test
    public void solution() {
        var code = IOUtil.input("Day05").get(0);
        assertEquals("answer 1", 6761139, answer(code, 1));
        assertEquals("answer 2", 9217546, answer(code, 5));
    }
}
