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
// hand made based on the map
        final var navigation = """
south
east
take boulder
west
north
west
take hypercube
west
take space law space brochure
west
north
take shell
west
take mug
south
take festive hat
north
east
south
east
east
east
east
north
west
north
take whirled peas
west
west
take astronaut ice cream
south
inv
""";
        for ( var c : navigation.toCharArray() ) {
            in.add( (long)c );
        }
        // A loud, robotic voice says "Alert! Droids on this ship are lighter than the detected value!" and you are ejected back to the checkpoint.

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
