package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.util.concurrent.LinkedBlockingQueue;

import static aoc19.computer.AsciiUtil.toIn;
import static org.junit.Assert.assertEquals;

public class Day21Test {

    static long run(String code) {
        final var memory = IntcodeComputer.loadMemory(IOUtil.input("day21").getFirst());
        final var in = new LinkedBlockingQueue<Long>();
        final var out = new LinkedBlockingQueue<Long>();
        toIn(in, code);

        final var comp = new IntcodeComputer(memory, in, out);
        comp.run();
        long last = 0;
        while (!out.isEmpty()) {
            last = out.poll();
        }
        return last;
    }

    @Test
    public void solution() {

        final var part1 = """
                NOT A J
                NOT B T
                AND D T
                OR T J
                NOT C T
                AND A T
                AND B T
                AND D T
                OR T J
                WALK
                """;

        final var part2 = """
                OR E J
                OR H J
                AND A J
                AND B J
                AND D J
                NOT C T
                AND T J
                NOT A T
                OR T J
                NOT B T
                AND D T
                OR T J
                RUN
                """;
        assertEquals("answer 1", 19357335, run(part1));
        assertEquals("answer 2", 1140147758, run(part2));
    }
}
