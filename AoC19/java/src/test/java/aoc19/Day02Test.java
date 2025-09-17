package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Assert;
import org.junit.Test;

import java.util.concurrent.LinkedBlockingQueue;

import static org.junit.Assert.assertEquals;

public class Day02Test {

    void run(String code, String memoryAfter) {
        final var memory = IntcodeComputer.loadMemory(code);
        var in = new LinkedBlockingQueue<Long>();
        var out = new LinkedBlockingQueue<Long>();
        var c = new IntcodeComputer(memory, in, out);
        c.run();
        Assert.assertArrayEquals("post run memory",
                IntcodeComputer.loadMemory(memoryAfter).toArray(),
                c.getMemory().toArray());

    }

    long run(String code, int noun, int verb) {
        final var memory = IntcodeComputer.loadMemory(code);
        memory.set(1, (long) noun);
        memory.set(2, (long) verb);
        var in = new LinkedBlockingQueue<Long>();
        var out = new LinkedBlockingQueue<Long>();
        var c = new IntcodeComputer(memory, in, out);
        c.run();
        return c.get(0);
    }

    @Test
    public void solution() {
        // run("1,0,0,3,99");
        run("1,9,10,3,2,3,11,0,99,30,40,50", "3500,9,10,70,2,3,11,0,99,30,40,50");
        run("1,0,0,0,99", "2,0,0,0,99");
        run("2,3,0,3,99", "2,3,0,6,99");
        run("2,4,4,5,99,0", "2,4,4,5,99,9801");
        run("1,1,1,4,99,5,6,0,99", "30,1,1,4,2,5,6,0,99");

        final var code = IOUtil.input("Day02").getFirst();
        assertEquals("answer 1", run(code, 12, 2), 3716250L);

        for (int noun = 0; noun <= 99; noun++) {
            for (int verb = 0; verb <= 99; verb++) {
                if (19690720 == run(code, noun, verb)) {
                    assertEquals("answer 2", 100 * noun + verb, 6472L);
                    return;
                }
            }
        }
    }
}
