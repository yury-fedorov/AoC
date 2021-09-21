package aoc16.d20;

import aoc16.common.IOUtil;
import org.junit.Assert;
import org.junit.Test;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.regex.Pattern;

public class Day20Test {

    static record MinMax(long min, long max) {}

    static long count( MinMax r ) {
        Assert.assertTrue( r.min() <= r.max() );
        return r.max() - r.min() + 1L;
    }

    final static long maxInt = 4294967295L;

    @Test
    public void solution() {
        final var input = IOUtil.input("d20");
        final var pattern = Pattern.compile( "^(\\d+)-(\\d+)$" );
        final var list = new ArrayList<MinMax>( input.size() );
        for ( final var l : input ) {
            final var m = pattern.matcher(l);
            Assert.assertTrue( "Bad syntax: " + l, m.find() );
            final long min = Long.parseLong( m.group(1) );
            final long max = Long.parseLong( m.group(2) );
            list.add( new MinMax(min, max ) );
        }
        list.sort(Comparator.comparing(MinMax::min));

        while ( true ) {
            boolean isCompressed = false;
            for ( int i = 0; i < list.size(); i++ ) {
                for ( int j = i + 1; j < list.size(); j++ ) {
                    final var r0 = list.get(i);
                    final var r1 = list.get(j);
                    Assert.assertTrue( r0.min() <= r1.min() );
                    if ( r0.max() >= ( r1.min() - 1 ) ) {
                        list.remove(j);
                        list.set(i, new MinMax( r0.min(), Math.max( r0.max(), r1.max() ) ) );
                        isCompressed = true;
                    }
                }
            }
            if (!isCompressed) break;
        }

        final long answer1 = list.get(0).min() == 0 ? list.get(0).max() + 1 : 0L;
        Assert.assertEquals( "answer 1", 32259706L, answer1 );

        final var blackListed = list.stream().mapToLong(Day20Test::count).sum();
        final long answer2 = maxInt + 1L - blackListed;

        Assert.assertEquals("answer 2", 113L, answer2 );
    }
}