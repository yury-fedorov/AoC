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
        final long _n;
        public DealWithIncrement( long size, long n ) { super( size ); _n = n; }
        long getNewPosition( long oldPosition ) { return ( oldPosition + _n ) % _size; } // TODO - implement
        long getOldPosition( long newPosition ) { return ( newPosition - _n ) % _size; } // TODO - implement
    }

    @Test
    public void solution() {

        final var d7 = createDeck( 7 );
        // cutCards(d7, 3); // 3,4,5,6,0,1,2 -- 3 cards from head moved to tail
        // cutCards(d7,-2); // 5,6,0,1,2,3,4 -- 2 cards from tail moved to head
        final var input = IOUtil.input("day22");
        final var SIZE1 = 10007;
        final var SIZE2  = 119_315_717_514_047L;
        final var TIMES2 = 101_741_582_076_661L;

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
                if ( command.startsWith( NEW_STACK ) || command.startsWith( "cut" ) ) {
                    final var after = deck.get( (int)afterPosition );
                    assertEquals( command, before, after );
                    assertEquals( "index inverse", shuffling.getOldPosition( afterPosition ), testPosition );
                }
            }
            fastCommands.add( shuffling );
        }
        // what is the position of card 2019?
        assertEquals( "answer 1", 6831, deck.indexOf(2019));

        // TODO assertEquals( "answer 2", -2, deck.get(2020-1).intValue() );
    }
}
