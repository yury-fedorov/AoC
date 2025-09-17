package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.util.ArrayList;
import java.util.Optional;
import java.util.concurrent.LinkedBlockingQueue;

import static org.junit.Assert.assertEquals;

public class Day23Test {

    @Test
    public void solution() {
        final var code = IntcodeComputer.loadMemory(IOUtil.input("day23").getFirst());
        final var COMPUTER_COUNT = 50;
        final var compList = new ArrayList<IntcodeComputer>(COMPUTER_COUNT);
        for (var address = 0L; address < COMPUTER_COUNT; address++) {
            final var memory = new ArrayList<>(code);
            final var in = new LinkedBlockingQueue<Long>();
            final var out = new LinkedBlockingQueue<Long>();
            in.add(address);
            final var comp = new IntcodeComputer(memory, in, out);
            compList.add(comp);
        }

        Optional<Long> answer1 = Optional.empty();
        Optional<Long> answer2 = Optional.empty();

        Optional<Long> natX = Optional.empty();
        Optional<Long> natY = Optional.empty();

        Optional<Long> previousNatY = Optional.empty();

        while (answer2.isEmpty()) {
            boolean isNetworkIdle = true;
            for (var address = 0; address < COMPUTER_COUNT; address++) {
                final var comp = compList.get(address);
                if (comp.inSize() == 0) {
                    comp.in(-1);
                } else {
                    isNetworkIdle = false;
                }
                comp.run();
                while (comp.outSize() >= 3) {
                    isNetworkIdle = false;
                    final var receiver = comp.out();
                    final var x = comp.out();
                    final var y = comp.out();
                    if (receiver == 255) {
                        if (answer1.isEmpty()) answer1 = Optional.of(y);
                        natX = Optional.of(x);
                        natY = Optional.of(y);
                        break;
                    } else if (receiver >= 0 && receiver < COMPUTER_COUNT) {
                        final var rc = compList.get((int) receiver);
                        rc.in(x);
                        rc.in(y);
                    }
                }
            }
            if (isNetworkIdle) {
                if (previousNatY.isPresent() && previousNatY.get().equals(natY.get())) {
                    answer2 = natY;
                } else {
                    final var c0 = compList.getFirst();
                    c0.in(natX.get());
                    c0.in(natY.get());
                    previousNatY = natY;
                }
            }
        }

        assertEquals("answer 1", 23259, answer1.get().longValue());
        assertEquals("answer 2", 15742, answer2.get().longValue());
    }
}
