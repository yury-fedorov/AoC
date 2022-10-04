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

    @Test
    public void solution() {
        final var input = IOUtil.input("day22");
        var deck = createDeck(10007);
        for ( var command : input ) {
            if ( command.equals(NEW_STACK) ) dealIntoNewStack(deck);
            else if ( command.startsWith( "cut ") ) {
                final var n = Integer.parseInt( command.split(" " )[1] );
                cutCards(deck, n);
            } else if ( command.startsWith( "deal with increment" ) ) {
                final var n = Integer.parseInt( command.split(" increment " )[1] );
                dealWithIncrement(deck, n);
            }
        }
        // what is the position of card 2019?
        assertEquals( "answer 1", 6831, deck.indexOf(2019));

        final var SIZE2  = 119_315_717_514_047L;
        final var TIMES2 = 101_741_582_076_661L;
        // deck = createDeck(SIZE2);
        // TODO assertEquals( "answer 2", -2, deck.get(2020-1).intValue() );
    }
}
