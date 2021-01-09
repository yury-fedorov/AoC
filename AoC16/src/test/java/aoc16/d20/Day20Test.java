package aoc16.d20;

import aoc16.common.IOUtil;
import org.javatuples.Pair;
import org.junit.Assert;
import org.junit.Test;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.regex.Pattern;

public class Day20Test {

    static long count( Pair<Long,Long> r ) {
        Assert.assertTrue( r.getValue0() <= r.getValue1() );
        return r.getValue1() - r.getValue0() + 1L;
    }

    final static long maxInt = 4294967295L;

    @Test
    public void solution() {
        final var input = IOUtil.input("d20");
        final var pattern = Pattern.compile( "^(\\d+)-(\\d+)$" );
        final var list = new ArrayList<Pair<Long,Long>>( input.size() );
        for ( final var l : input ) {
            final var m = pattern.matcher(l);
            Assert.assertTrue( "Bad syntax: " + l, m.find() );
            final long min = Long.parseLong( m.group(1) );
            final long max = Long.parseLong( m.group(2) );
            list.add( Pair.with(min, max ) );
        }
        list.sort(Comparator.comparing(Pair::getValue0));

        while ( true ) {
            boolean isCompressed = false;
            for ( int i = 0; i < list.size(); i++ ) {
                for ( int j = i + 1; j < list.size(); j++ ) {
                    final var r0 = list.get(i);
                    final var r1 = list.get(j);
                    Assert.assertTrue( r0.getValue0() <= r1.getValue0() );
                    if ( r0.getValue1() >= ( r1.getValue0() - 1 ) ) {
                        list.remove(j);
                        list.set(i, Pair.with( r0.getValue0(), Math.max( r0.getValue1(), r1.getValue1() ) ) );
                        isCompressed = true;
                    }
                }
            }
            if (!isCompressed) break;
        }

        final long answer1 = list.get(0).getValue0() == 0 ? list.get(0).getValue1() + 1 : 0L;
        Assert.assertEquals( "answer 1", 32259706L, answer1 );

        final var blackListed = list.stream().mapToLong(Day20Test::count).sum();
        final long answer2 = maxInt + 1L - blackListed;

        Assert.assertEquals("answer 2", 113L, answer2 );
    }
}