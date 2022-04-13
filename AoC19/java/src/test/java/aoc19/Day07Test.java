package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;
import org.apache.commons.collections4.iterators.PermutationIterator;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;

import static org.junit.Assert.assertEquals;

public class Day07Test {

    // possible to be used only for task 1
    static long singleRun(ArrayList<Long> code, long setting, long input) {
        var in = new LinkedBlockingQueue<Long>();
        in.add(setting);
        in.add(input);
        var out = new LinkedBlockingQueue<Long>();
        IntcodeComputer.run(code, in, out);
        return out.poll();
    }

    static long allRuns1( ArrayList<Long> code, List<Long> settings ) {
        long input = 0;
        for ( var setting : settings ) {
            input = singleRun( code, setting, input );
        }
        return input;
    }

    static long allRuns2( ArrayList<Long> code, List<Long> settings ) {
        boolean isHalt = false;
        var ampList = new ArrayList<IntcodeComputer>();
        for ( var setting : settings ) {
            var in = new LinkedBlockingQueue<Long>();
            var out = new LinkedBlockingQueue<Long>();
            in.add(setting);
            var amp = new IntcodeComputer(code, in, out);
            ampList.add(amp);
        }
        ampList.get(0).in(0); // the very first input is zero
        var n = settings.size();
        var lastAmp = ampList.get( n - 1 );
        do {
            for ( var i = 0; i < n; i++ ) {
                var amp = ampList.get(i);
                isHalt = amp.run() == IntcodeComputer.RunPhase.HALT;
                if ( !isHalt ) {
                    var nextIndex = ( i + 1 ) % n;
                    var nextAmp = ampList.get(nextIndex);
                    if ( amp.isOut() )
                    {
                        var out = amp.out();
                        nextAmp.in(out);
                    }
                }
            }
        } while ( !isHalt );
        return lastAmp.out();
    }

    static long solution( ArrayList<Long> code, List<Long> settings, boolean isTask1 ) {
        long result = Long.MIN_VALUE;
        var i = new PermutationIterator(settings);
        while (i.hasNext()) {
            var curSettings = i.next();
            final long r = isTask1 ? allRuns1(code, curSettings) : allRuns2(code, curSettings);
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
