package aoc16.d19;

import org.junit.Assert;
import org.junit.Test;

import java.util.LinkedList;
import java.util.ListIterator;

public class Day19Test {

    static ListIterator<Integer> next( LinkedList<Integer> list, ListIterator<Integer> it ) {
        if ( !it.hasNext() ) it = list.listIterator();
        it.next();
        return it;
    }

    static ListIterator<Integer> prev( LinkedList<Integer> list, ListIterator<Integer> it ) {
        if ( !it.hasPrevious() ) {
            while ( it.hasNext() ) it.next();
        }
        it.previous();
        return it;
    }

    static ListIterator<Integer> next( LinkedList<Integer> list, ListIterator<Integer> it, int n ) {
        final boolean isNext = n > 0;
        for ( int i = Math.abs(n); --i >= 0; ) { it = isNext ? next(list, it) : prev( list, it ); }
        return it;
    }

    static int detectWinner( final int n ) {
        final LinkedList<Integer> list = init(n);
        var it = next(list, list.listIterator() );
        while (list.size() > 1) {
            it = next(list, it);
            it.remove();
            it = next(list, it); // remove() call brings us to the previous element
        }
        return list.get(0);
    }

    static int detectWinner2( final int n ) {
        final LinkedList<Integer> list = init(n);
        var it = next(list, list.listIterator() );
        while (list.size() > 1) {
            final var half = ( list.size() >> 1 ); // divide by 2
            it = next(list, it, half);
            it.remove();
            it = next(list, it, -( half - 2 ) ); // we moved one back and we need to move to the next element
        }
        return list.get(0);
    }

    private static LinkedList<Integer> init(int n) {
        final var list = new LinkedList<Integer>();
        for (int i = 1; i <= n; i++ ) {
            list.addLast(i);
        }
        return list;
    }

    @Test
    public void solution() {
        final var INPUT = 3018458;
        Assert.assertEquals( "test 1", 3, detectWinner(5));
        Assert.assertEquals( "answer 1", 1842613, detectWinner(INPUT));
        Assert.assertEquals( "test 2", 2, detectWinner2(5));
        // Assert.assertEquals( "answer 2", -1, detectWinner2(INPUT));
    }
}
