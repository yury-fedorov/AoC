package aoc19;

import org.junit.Test;

import java.util.*;
import java.util.stream.*;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class Day16Test {
    // 0, 1, 0, -1
    // 1, 0, -1, 0
    // 0, 1, 1, 0, 0, -1, -1, 0, 0, 1, 1, 0, 0, -1, -1
    // 0, 0, 1, 1, 1, 0, 0, 0, -1, -1, -1.
    static List<Integer> pattern(int n, int length) {
        var p = new ArrayList<Integer>(length);
        var times = n + 1;
        final var SEQ = List.of( 0, 1, 0, -1 );
        while ( p.size() <= length ) {
            for ( var pe : SEQ ) {
                p.addAll( Collections.nCopies(times, pe) );
            }
        }
        return p.subList(1, length + 1);
    }

    static List<Integer> fft( List<Integer> signal ) {
        final var length = signal.size();
        final var result = new ArrayList<Integer>(length);
        for ( int pos = 0; pos < length; pos++ ) {
            int value = 0;
            final var p = pattern( pos, length );
            for ( int i = 0; i < length; i++ ) {
                final int pi = p.get(i);
                final int si = signal.get(i);
                switch( pi ) {
                    case 1: value += si; break;
                    case -1:  value -= si; break;
                    case 0: break;
                    default: throw new IllegalStateException( "not expacted value");
                }
            }
            result.add( Math.abs( value % 10 ) );
        }
        return result;
    }

    // 1*1  + 2*0  + 3*-1 + 4*0  + 5*1  + 6*0  + 7*-1 + 8*0 = -4 = 4
    // 1*0  + 2*1  + 3*1  + 4*0  + 5*0  + 6*-1 + 7*-1 + 8*0 = -8 = 8
    // 1*0  + 2*0  + 3*1  + 4*1  + 5*1  + 6*0  + 7*0  + 8*0 = 12 = 2
    @Test
    public void solution() {
        assertEquals( List.of(1,0,-1,0), pattern(0,4) );
        assertEquals( List.of(0, 1, 1, 0, 0, -1, -1, 0, 0, 1, 1, 0, 0, -1, -1), pattern(1,15) );
        assertEquals( List.of(0, 0, 1, 1, 1, 0, 0, 0, -1, -1, -1), pattern(2,11) );
        assertEquals( List.of( 4,8,2,2,6,1,5,8 ), fft( List.of(1,2,3,4,5,6,7,8) ) );
        final String signal = IOUtil.input("day16").get(0);
        List<Integer> s = signal.chars().map( c -> c - '0' )
            .collect(ArrayList<Integer>::new, ArrayList::add, ArrayList::addAll);

        final boolean isPart2 = false;

        final var offset = s.subList(0,7);
        final String strOffset = offset.stream().map( i -> (char)('0' + i))
            .collect(Collector.of(
                StringBuilder::new,
                StringBuilder::append,
                StringBuilder::append,
                StringBuilder::toString));

        final int intOffset = Integer.parseInt( strOffset );

        // preparation of input for answer 2
        final var n = 10_000;
        List<Integer> s2 = new ArrayList( s.size() * n );
        for ( int i = 0; i < n; i++ ) {
            s2.addAll( s );
        }

        for ( int i = 0; i < 100; i++ ) {
            s = fft( s );   
            if ( isPart2 ) s2  = fft( s2 );        
        }
        assertEquals( "answer 1", List.of(8, 4, 4, 8, 7, 7, 2, 4), s.subList(0,8) );
        if ( isPart2 ) assertEquals( "answer 2", List.of(0), s2.subList( intOffset, intOffset + 8 ) );
    }
}