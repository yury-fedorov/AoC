package aoc19;

import org.junit.Test;

import java.util.*;
import java.util.stream.*;

import static org.junit.Assert.assertEquals;

public class Day16Test {

    record ValueTimes ( int value, int times ) {}

    // optimized by size
    static List<ValueTimes> pattern1(int n, int length) {
        final var SEQ = List.of( 0, 1, 0, -1 );
        var times = n + 1;
        var result = new ArrayList<ValueTimes>();
        var index = 0;
        var e = new ValueTimes( SEQ.get(index), times - 1 ); // very first element
        if ( e.times > 0 ) result.add(e);
        var size = e.times;
        while ( size <= length ) {
            index = ( index + 1 ) % SEQ.size();
            e = new ValueTimes( SEQ.get(index), times );
            size += e.times;
            result.add(e);
        }
        final var de = size - length;
        if ( de > 0 ) {
            final var last = result.remove( result.size() - 1 );
            final var times1 = last.times - de;
            if (times1 > 0) result.add( new ValueTimes( last.value, times1 ) );
        }
        return result;
    }

    static int getPattern1( List<ValueTimes> p, int i ) {
        var min = 0;
        for ( var pi : p ) {
            final var max = min + pi.times - 1;
            if ( i >= min && i <= max ) return pi.value;
            min = max + 1;
        }
        throw new ArrayIndexOutOfBoundsException();
    }

    // 0, 1, 0, -1
    // 1, 0, -1, 0
    // 0, 1, 1, 0, 0, -1, -1, 0, 0, 1, 1, 0, 0, -1, -1
    // 0, 0, 1, 1, 1, 0, 0, 0, -1, -1, -1
    // first, inefficient implementation
    static List<Integer> pattern(int n, int length) {
        final var p = new ArrayList<Integer>(length);
        final var times = n + 1;
        final var SEQ = List.of( 0, 1, 0, -1 );
        while ( p.size() <= length ) {
            for ( var pe : SEQ ) {
                p.addAll( Collections.nCopies(times, pe) );
            }
        }
        return p.subList(1, length + 1);
    }
    @Test
    public void testPatterns() {
        for ( var l : List.of(1, 2, 4, 8, 16) ) {
            for ( var n : List.of(0,1,2,3,5,8,13) ) {
                testPattern(n, l );
            }
        }
    }

    void testPattern(int n, int length) {
        final var p = pattern(n,length);
        final var p1 = pattern1(n,length);
        for ( var i = 0; i < length; i++ ) {
            final int a = p.get(i);
            final int b = getPattern1(p1, i);
            if ( a != b )
                assertEquals( a, b );
        }
    }

    static List<Integer> fft( List<Integer> signal ) {
        return fftRange(signal, 0, signal.size());
    }

    record RangeValue (int min, int max, int value) {}

    static List<RangeValue> fftComposition(int pos, int length) {
        final var p = pattern1( pos, length );
        final var result = new ArrayList<RangeValue>(p.size());
        var min = 0;
        for ( var pi : p ) {
            final var max = min + pi.times - 1; // [min, max]
            if ( pi.value != 0 ) {
                result.add( new RangeValue( min, max, pi.value ) );
            }
            min = max + 1;
        }
        return result;
    }

    static int fftValue( List<Integer> signal, List<RangeValue> composition ) {
        var value = 0;
        for ( var rv : composition ) {
            for ( var index = rv.min; index <= rv.max; index++ ) {
                final var s = signal.get( index );
                value += rv.value * s;
            }
        }
        return Math.abs( value % 10 );
    }

    static List<Integer> fftRange( List<Integer> signal, int pos0, int pos1 ) {
        final var length = signal.size();
        final var cache = new HashMap<Integer, List<RangeValue>>();
        for ( int pos = pos0; pos < pos1; pos++ ) {
            final var c = fftComposition( pos, length );
            cache.put(pos, c);
        }
        final var min = cache.values().stream().mapToInt(
                l -> l.stream().mapToInt( rv -> rv.min ).min().getAsInt() ).min().getAsInt();
        final var max = cache.values().stream().mapToInt(
                l -> l.stream().mapToInt( rv -> rv.max ).max().getAsInt() ).max().getAsInt();
        final var result = new ArrayList<>( signal );
        for ( int pos = Math.max(min - 101,0); pos <= max; pos++ ) {
            final var c = fftComposition( pos, length );
            result.set( pos, fftValue( signal, c ) );
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
            .collect(ArrayList::new, ArrayList::add, ArrayList::addAll);

        final boolean isPart2 = !Config.isFast(); // part 2 is SLOW, it takes 4 h 24 m

        final var offset = s.subList(0,7);
        final String strOffset = offset.stream().map( i -> (char)('0' + i))
            .collect(Collector.of(
                StringBuilder::new,
                StringBuilder::append,
                StringBuilder::append,
                StringBuilder::toString));

        final int intOffset = Integer.parseInt( strOffset );
        final int offsetEnd = intOffset + 8;

        // preparation of input for answer 2
        final var n = 10_000;
        List<Integer> s2 = new ArrayList<>( s.size() * n );
        for ( int i = 0; i < n; i++ ) {
            s2.addAll( s );
        }

        for ( int i = 0; i < 100; i++ ) {
            if ( !isPart2 ) s = fft( s );
            else s2  = fftRange( s2, intOffset, offsetEnd );
        }
        if ( !isPart2 ) assertEquals( "answer 1", List.of(8, 4, 4, 8, 7, 7, 2, 4), s.subList(0,8) );
        else assertEquals( "answer 2", List.of(8, 4, 6, 9, 2, 5, 2, 4), s2.subList( intOffset, offsetEnd) );
    }
}
