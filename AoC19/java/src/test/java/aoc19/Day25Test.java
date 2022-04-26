package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;

import static org.junit.Assert.assertEquals;

public class Day25Test {

    @Test
    public void solution()  {
        assertEquals("answer 1", -1, 0 );
    }

    public static void main(String [] a) throws IOException {
        final var code = IntcodeComputer.loadMemory(IOUtil.input("day25").get(0));
        final var in = new LinkedBlockingQueue<Long>();
        final var out = new LinkedBlockingQueue<Long>();
        final var comp = new IntcodeComputer(code, in, out);

        while ( comp.run() != IntcodeComputer.RunPhase.HALT ) {
            out(out);
            final var br = new BufferedReader(new InputStreamReader(System.in));
            final var cmd = br.readLine();
            for ( var c : cmd.toCharArray() ) {
                in.add((long)c);
            }
            in.add(10L);
        }
        out(out);
    }

    static void out( Queue<Long> out ) {
        final var text = new StringBuilder();
        while ( !out.isEmpty() ) {
            final long c = out.poll();
            text.append((char)c);
        }
        System.out.println(text);
    }
}
