package aoc19;

import org.junit.Test;

import java.util.*;
import java.util.stream.IntStream;

import static org.junit.Assert.assertEquals;

public class Day22Test {

    static List<Integer> createDeck() {
        return new LinkedList<>(IntStream.rangeClosed(0,10006).boxed().toList());
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
        final var set = new HashSet<Integer>(size);
        while ( !deck.isEmpty() ) {
            final var head = deck.remove(0);
            if ( !set.contains(index) ) {
                while ( result.size() <= index ) result.add(0);
                result.set(index, head);
                set.add(index);
            }
            index = ( index + 3 ) % size;
         }
        deck.addAll(result);
    }

    @Test
    public void solution() {
        final var input = IOUtil.input("day22");
        var deck = createDeck();
        for ( var command : input ) {
            if ( command.contains(NEW_STACK) ) dealIntoNewStack(deck);
            else if ( command.contains( "cut ") ) {
                final var n = Integer.parseInt( command.split(" " )[1] );
                cutCards(deck, n);
            } else if ( command.contains( "deal with increment" ) ) {
                final var n = Integer.parseInt( command.split(" increment " )[1] );
                dealWithIncrement(deck, n);
            }
        }
        // what is the position of card 2019?
        assertEquals( "answer 1", -1, deck.indexOf(2019)); // 5931 is too low
        assertEquals( "answer 2", -2, 0 );
    }
}
