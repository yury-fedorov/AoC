package aoc19;

import org.junit.Test;

import java.util.*;
import java.util.stream.IntStream;

import static org.junit.Assert.assertEquals;

public class Day22Test {

    static List<Integer> createDeck(int size) {
        return new LinkedList<>(IntStream.range(0,size).boxed().toList());
    }
    static final String NEW_STACK = "deal into new stack";
    static void dealIntoNewStack( List<Integer> deck ) { Collections.reverse(deck); }

    static void cutCards( List<Integer> deck, int n ) {
        if ( n > 0 ) {
            for ( var i = 0; i < n; i++ ) {
                final var head = deck.remove(0);
                deck.add(head);
            }
        } else {
            n = -n;
            final var lastIndex = deck.size() - 1;
            for ( var i = 0; i < n; i++ ) {
                final var tail = deck.remove(lastIndex);
                deck.add(0, tail);
            }
        }
    }

    static void dealWithIncrement( List<Integer> deck, int n ) {
        int index = 0;
        final var size = deck.size();
        final var result = new ArrayList<Integer>(size);

        while ( !deck.isEmpty() ) {
            final var head = deck.remove(0);
            while ( result.size() <= index ) result.add(Integer.MIN_VALUE);
            result.set(index, head);
            index = ( index + n ) % size;
        }
        deck.addAll(result);
    }

    static abstract class Shuffling {
        protected final long _size;
        protected Shuffling( long size ) { _size = size; }
        abstract long getNewPosition( long oldPosition ); // direct (described) shuffling
        abstract long getOldPosition( long newPosition ); // inverse shuffling
    }

    static class DealIntoNewStack extends Shuffling {
        public DealIntoNewStack( long size ) { super( size ); }
        long getNewPosition( long oldPosition ) { return _size - oldPosition - 1; }
        long getOldPosition( long newPosition ) { return getNewPosition( newPosition ); }
    }

    static class CutCards extends Shuffling {
        final long _n;
        public CutCards( long size, long n ) { super( size ); _n = n; }

        long makePositive(long value) { return value < 0 ? value + _size : value; }
        long getNewPosition( long oldPosition ) { return makePositive( ( oldPosition - _n ) % _size ); }
        long getOldPosition( long newPosition ) { return makePositive( ( newPosition + _n ) % _size ); }
    }

    static class DealWithIncrement extends Shuffling {
        final long _n; // n is de facto in the range: [2, 72]
        // every iteration (we have n) starts with a new shift (unique in the range 0 [0,n) )
        final Map<Long,Long> _shiftToIteration = new HashMap<>();
        public DealWithIncrement( long size, long n ) { super( size ); _n = n; }
        long getNewPosition( long oldPosition ) { return oldPosition * _n % _size; }
        
        long getIteration( long shift ) {
            final long DEFAULT = -1L;
            long result = _shiftToIteration.getOrDefault(shift, DEFAULT);
            if ( result == DEFAULT ) {
                // initialization of the map
                final long iteration = _size / _n;
                long position = iteration / 2;
                for ( var i = 0; i < _n; i++ ) {
                    final var pn = position * _n;
                    final long newPosition = pn % _size;
                    final long posShift = newPosition % _n;
                    _shiftToIteration.put( posShift, (long)i );
                    position += iteration;
                }
                result = _shiftToIteration.get(shift);
            }
            return result;
        }
        
        long getOldPosition( long newPosition ) {
            final var shift = newPosition % _n; // shift
            final var iteration = getIteration( shift );
            return ( newPosition + _size * iteration ) / _n;
        }
    }
    
    void testDealWithIncrease(int size, int n) {
        final var d = createDeck( size );
        final var c = "deal with increment " + n;
        final Shuffling s = new DealWithIncrement(size, n);
        dealWithIncrement(d, n);
        for ( int before = 0; before < size; before++ )
        {
            final int testPosition = before;
            final var afterPosition = s.getNewPosition(testPosition);
            final int after = d.get( (int)afterPosition );
            assertEquals( c + " before vs after -> before=" + before + " after=" + after, before, after );
            assertEquals( c + " index inverse -> after position=" + afterPosition, s.getOldPosition( afterPosition ), testPosition );
        }
    }

    @Test
    public void solution() {
        final var SIZE1 = 10007;
        // java long max = 9,223,372,036,854,775,807 -> * by [2,72] will not lead to overflow
        final var SIZE2  =       119_315_717_514_047L;
        final var TIMES2 = 101_741_582_076_661L;
        
        testDealWithIncrease( 19, 3 );
        testDealWithIncrease( 7, 2 );
        testDealWithIncrease( SIZE1, 46 );

        final var input = IOUtil.input("day22");
        final var deck = createDeck(SIZE1);
        final var fastCommands = new ArrayList<Shuffling>( input.size() );
        final var size = SIZE2;
        for ( var command : input ) {
            final Shuffling shuffling;
            if ( command.equals(NEW_STACK) ) {
                dealIntoNewStack(deck);
                shuffling = new DealIntoNewStack( size );
            } else if ( command.startsWith( "cut ") ) {
                final var n = Integer.parseInt( command.split(" " )[1] );
                cutCards(deck, n);
                shuffling = new CutCards( size, n );
            } else if ( command.startsWith( "deal with increment" ) ) {
                final var n = Integer.parseInt( command.split(" increment " )[1] );
                dealWithIncrement(deck, n);
                shuffling = new DealWithIncrement( size, n );
            } else throw new IllegalStateException();
            fastCommands.add( shuffling );
        }
        // what is the position of card 2019?
        assertEquals( "answer 1", 6831, deck.indexOf(2019));
        
        if ( Config.isFast() ) return;
        // TODO - in debug it is super slow (up to 2,100 days)
        Collections.reverse(fastCommands);
        long position = 2020 - 1;
        for ( var i = 0L; i < TIMES2; i++ ) {
            for ( var c : fastCommands ) {
                position = c.getOldPosition( position );
            }
        }
        assertEquals( "answer 2", -2L, position );
    }
}
