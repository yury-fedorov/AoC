package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.util.ArrayList;
import java.util.Optional;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;

import static org.junit.Assert.assertEquals;

public class Day23Test {

    @Test
    public void solution() {
        final var code = IntcodeComputer.loadMemory(IOUtil.input("day23").get(0));
        final var COMPUTER_COUNT = 50;
        final var compList = new ArrayList<IntcodeComputer>(COMPUTER_COUNT);
        final var outList = new ArrayList<Queue<Long>>(COMPUTER_COUNT);
        for ( var address = 0L; address < COMPUTER_COUNT; address++ ) {
            final var memory = new ArrayList<>(code);
            final var in = new LinkedBlockingQueue<Long>();
            final var out = new LinkedBlockingQueue<Long>();
            in.add(address);
            final var comp = new IntcodeComputer(memory, in, out);
            compList.add(comp);
            outList.add(out);
        }

        Optional<Long> answer1 = Optional.empty();
        while ( answer1.isEmpty() ) {
            for ( var address = 0; address < COMPUTER_COUNT; address++ ) {
                final var comp = compList.get(address);
                final var out = outList.get(address);
                final var r = comp.run();
                if ( r == IntcodeComputer.RunPhase.NEED_FOR_INPUT ) {
                    comp.in(-1 );
                }
                while ( out.size() >= 3 ) {
                    final var receiver = out.poll();
                    final var x = out.poll();
                    final var y = out.poll();
                    if ( receiver == 255 ) {
                        if ( answer1.isEmpty() ) answer1 = Optional.of(y);
                        break;
                    } else if ( receiver >= 0 && receiver < COMPUTER_COUNT ) {
                        final var rc = compList.get( receiver.intValue() );
                        rc.in(x);
                        rc.in(y);
                    }
                }
                if ( answer1.isPresent() ) break;
            }
        }

        assertEquals("answer 1", 23259, answer1.get().longValue() );
    }
}
