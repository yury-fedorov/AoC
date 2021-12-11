package aoc19;

import aoc19.computer.IntcodeComputer;
import aoc19.computer.operation.NothingOnEvent;
import org.junit.Ignore;
import org.junit.Test;

import java.util.ArrayList;
import java.util.concurrent.LinkedBlockingQueue;

import static org.junit.Assert.assertEquals;

public class Day09Test {

    @Ignore("failing")
    @Test
    public void test() {
        final var code = "109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99"; // copy of itself
        final var  memory = IntcodeComputer.loadMemory(code);
        var in = new LinkedBlockingQueue<Long>();
        var out = new LinkedBlockingQueue<Long>();
        IntcodeComputer.run( memory, in, out, new NothingOnEvent());
        var asArray = new ArrayList<Long>( out );
        assertEquals( "copy of itself", IntcodeComputer.loadMemory(code), asArray );
    }

    @Test // good
    public void testLargeNumberInMiddle() {
        final var code = "104,1125899906842624,99";
        final var  memory = IntcodeComputer.loadMemory(code);
        var in = new LinkedBlockingQueue<Long>();
        var out = new LinkedBlockingQueue<Long>();
        IntcodeComputer.run( memory, in, out, new NothingOnEvent());
        assertEquals(1, out.size());
        assertEquals( 1125899906842624L, out.remove().longValue() );
    }

    @Test
    public void testSixteenDigitNumber() {
        final var code =  "1102,34915192,34915192,7,4,7,99,0";
        final var  memory = IntcodeComputer.loadMemory(code);
        var in = new LinkedBlockingQueue<Long>();
        var out = new LinkedBlockingQueue<Long>();
        IntcodeComputer.run( memory, in, out, new NothingOnEvent());
        assertEquals(1, out.size());
        long number = out.remove().longValue();
        int digits = 0;
        while ( number != 0 ) {
            digits++;
            number = number / 10;
        }
        assertEquals( "16 digits number", 16, digits );
    }

    @Ignore( "produces 203 but 1102 is too lows" )
    @Test
    public void solution() {
        final var  memory = IntcodeComputer.loadMemory(IOUtil.input("day09").get(0));
        var in = new LinkedBlockingQueue<Long>();
        var out = new LinkedBlockingQueue<Long>();
        in.add(1l);
        IntcodeComputer.run( memory, in, out, new NothingOnEvent());
        assertEquals( "answer 1", -1l, out.remove().longValue() ); //1102 too low
    }
}
