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
        final Map<Long,Long> _shiftToIteration = new HashMap<Long,Long>();
        public DealWithIncrement( long size, long n ) { super( size ); _n = n; }
        long getNewPosition( long oldPosition ) {
            final var pn = oldPosition * _n;
            final long iteration = pn / _size;
            final long newPosition = pn % _size;
            final long shift = newPosition % _n;
            _shiftToIteration.put( shift, iteration );
            return newPosition;
        }
        
        long getIteration( long shift ) {
            // TODO - trigger initialization of the map if necessary
            return _shiftToIteration.get( shift );
        }
        
        long getOldPosition( long newPosition ) {
            // TODO - this could be optimized and moved to class member fields
            final long sn = _size / _n;
            final long smn = _size % _n; // amount of times we have n+1 in one iteration
            
            final var shift = newPosition % _n; // shift
            final var iteration = getIteration( shift );
            
            // final var a = newPosition / _n;
            return ( ( newPosition + _size * iteration ) / _n ) /* % _size */; 
            // "oldPosition < s/n" -> newPosition / n
            // newPosition < n -> sn + newPosition
            // return ( a + ( b * sn ) + Math.min( smn, b ) ) % _size;
        }
    }
    
    void testDealWithIncrease(int size, int n) {
        final var d = createDeck( size );
        // cut cards for deck size 7
        // cutCards(d, 3); // 3,4,5,6,0,1,2 -- 3 cards from head moved to tail
        // cutCards(d,-2); // 5,6,0,1,2,3,4 -- 2 cards from tail moved to head
        final var c = "deal with increment " + n;
        final var s = new DealWithIncrement(size, n);
        dealWithIncrement(d, n); // 0,4,1,5,2,6,3
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
        // java long max = 9,223,372,036,854,775,807
        final var SIZE2  =       119_315_717_514_047L;
        final var TIMES2 = 101_741_582_076_661L;
        
        testDealWithIncrease( 7, 2 );
        testDealWithIncrease( 11, 3 );
        testDealWithIncrease( SIZE1, 46 );

        final var input = IOUtil.input("day22");
        final var deck = createDeck(SIZE1);
        final var fastCommands = new ArrayList<Shuffling>( input.size() );
        final var size = SIZE1;
        final var isFixedTestValue = false;
        final Optional<Integer> fixedTestPosition = isFixedTestValue ? Optional.of( 0 ) : Optional.empty();
        for ( var command : input ) {
            final Shuffling shuffling;
            final int testPosition = fixedTestPosition.orElse(
                    (int)Math.min( (long)( Math.random() * size ) % size, Integer.MAX_VALUE ) );
            final var before = deck.get( testPosition );
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
            final var afterPosition = shuffling.getNewPosition(testPosition);
            if ( afterPosition <= Integer.MAX_VALUE ) {
                // TODO - clean -> if ( command.startsWith( NEW_STACK ) || command.startsWith( "cut" ) )
                {
                    final var after = deck.get( (int)afterPosition );
                    assertEquals( command + " before vs after", before, after );
                    assertEquals( command + " index inverse", shuffling.getOldPosition( afterPosition ), testPosition );
                }
            }
            fastCommands.add( shuffling );
        }
        // what is the position of card 2019?
        assertEquals( "answer 1", 6831, deck.indexOf(2019));

        // TODO assertEquals( "answer 2", -2, deck.get(2020-1).intValue() );
    }
}
