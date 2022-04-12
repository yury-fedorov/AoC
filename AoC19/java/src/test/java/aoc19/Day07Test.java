package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;
import org.apache.commons.collections4.iterators.PermutationIterator;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;

import static org.junit.Assert.assertEquals;

public class Day07Test {
    record SingleRunResult( long result, boolean isHalt ) {}

    static SingleRunResult singleRun(ArrayList<Long> code, long setting, long input) {
        var in = new LinkedBlockingQueue<Long>();
        in.add(setting);
        in.add(input);
        var out = new LinkedBlockingQueue<Long>();
        final var isHalt = IntcodeComputer.run(code, in, out);
        return new SingleRunResult( out.poll(), isHalt );
    }

    static long allRuns( ArrayList<Long> code, List<Long> settings, boolean isTask1 ) {
        long input = 0;
        long maxInput = Long.MIN_VALUE;
        try {
            do {
                boolean isHalt = false;
                for ( var setting : settings ) {
                    var result = singleRun( code, setting, input );
                    input = result.result;
                    isHalt = result.isHalt;
                }
                maxInput = Math.max( input, maxInput );
                if (isHalt) break; // halt
            } while ( !isTask1 ); // task 2 till halt
        } catch (Exception e) {
            // ie. when there is no input
        }
        return maxInput;
    }

    static long solution( ArrayList<Long> code, List<Long> settings, boolean isTask1 ) {
        long result = Long.MIN_VALUE;
        var i = new PermutationIterator(settings);
        while (i.hasNext()) {
            var curSettings = i.next();
            final long r = allRuns(code, curSettings, isTask1);
            result = Math.max( result, r );
        }
        return result;
    }

    static long answer1( ArrayList<Long> code ) {
        var settings = List.of(0L,1L,2L,3L,4L);
        return solution(code, settings, true);
    }

    static long answer2( ArrayList<Long> code ) {
        var settings = List.of(5L,6L,7L,8L,9L);
        return solution(code, settings, false);
    }

    @Test
    public void solution() {
        final var code = IntcodeComputer.loadMemory(IOUtil.input("day07").get(0));
        assertEquals( "answer 1", 70597L, answer1(code) );
        assertEquals( "answer 2", -2L, answer2(code) );
    }
}
