package aoc16.d20;

import aoc16.common.Config;
import aoc16.common.IOUtil;
import org.javatuples.Pair;
import org.junit.Assert;
import org.junit.Test;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashSet;
import java.util.regex.Pattern;

public class Day20Test {

    static long count( Pair<Long,Long> r ) {
        Assert.assertTrue( r.getValue0() <= r.getValue1() );
        return r.getValue1() - r.getValue0() + 1L;
    }

    final static long maxInt = 4294967295L; // 0x00000000FFFFFFFF

    static long toInt( final long value ) {
        final var result = value & maxInt;
        Assert.assertTrue( result >= 0 );
        Assert.assertTrue( value >= result );
        return result;
    }

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
            for ( int i = 0; true; i++ ) {
                final int i1 = i + 1;
                if ( i1 >= list.size() ) break;
                final var r0 = list.get(i);
                final var r1 = list.get(i1);
                if ( r0.getValue1() >= ( r1.getValue0() - 1 ) ) {
                    list.remove(i1);
                    list.set(i, Pair.with( r0.getValue0(), r1.getValue1() ) );
                    isCompressed = true;
                }
            }
            if ( isCompressed == false ) break;
        }
        // integrity check
        for ( int i = 2; i < list.size(); i++ ) {
            final var r0 = list.get(i-1);
            final var r1 = list.get(i);
            Assert.assertTrue( r0.getValue1() < r1.getValue0() );
        }

        final long answer1 = list.get(0).getValue0() == 0 ? list.get(0).getValue1() + 1 : 0L;
        Assert.assertEquals( "answer 1", 32259706L, answer1 );

        Assert.fail( "not write answer" ); // TODO - second part does not work

        long badCount = 0L;
        long good = 0L;
        long cur = 0L;
        for ( final var r : list ) {
            final long min = r.getValue0();
            final long max = r.getValue1();
            badCount += count(r);
            for ( ; cur < min; cur++ ) good++;
            cur = max + 1;
        }
        for ( ; cur <= maxInt; cur++ ) good++;

        /*
        final var blackListed = list.stream()
                .map( (p) -> count(p) )
                .reduce(0L, Long::sum).longValue();

        final long answer2 = count( Pair.with(0L, maxInt ) ) - blackListed;
        */
        final long answer2 = good;
        Assert.assertTrue( "answer 2 is too high: " + answer2, answer2 < 836882514 );
        Assert.assertEquals("answer 2", -1, answer2 );
    }
}
