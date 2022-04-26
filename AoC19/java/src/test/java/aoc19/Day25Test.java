package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Arrays;
import java.util.List;
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
        toIn( in, navigation );
        final var things = getThings(navigation);
        // A loud, robotic voice says "Alert! Droids on this ship are lighter than the detected value!" and you are ejected back to the checkpoint.

        final var flagsLimits = (long)Math.pow(2, things.size());
        for ( var flags = 0L; flags < flagsLimits; flags++ ) {
            toIn( in, generate(things, flags) );
            comp.run();
            var lastOut = readOut(out).toString();
            if ( !lastOut.contains( "Droids on this ship are") ) {
                break;
                // Oh, hello! You should be able to get in by typing 33624080 on the keypad at the main airlock.
            }
        }

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

    static void toIn( Queue<Long> in, CharSequence commands ) {
        for ( var c : commands.toString().toCharArray() ) {
            in.add( (long)c );
        }
    }

    static List<String> getThings(String navigation) {
        return Arrays.stream(navigation.split( "\n" ))
                .filter(s -> s.contains("take "))
                .map( s -> s.substring( 5) )
                .toList();
    }

    static CharSequence readOut(Queue<Long> out ) {
        final var text = new StringBuilder();
        while ( !out.isEmpty() ) {
            final long c = out.poll();
            text.append((char)c);
        }
        return text;
    }

    static void out( Queue<Long> out ) {
        System.out.println( readOut(out) );
    }

    static CharSequence generate( List<String> things, long flags ) {
        final var text = new StringBuilder();
        for ( var t : things ) text.append( "drop ").append( t ).append( '\n' ); // drop everything
        for ( var i = things.size(); --i >= 0; ) {
            final var t = things.get(i);
            final var flag = 1 << i;
            if ( ( flags & flag ) != 0 )
                text.append("take ").append(t).append('\n');
        }
        text.append("south\n");
        return text;
    }
}
