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

    static int detectWinner( final int n ) {
        final var list = new LinkedList<Integer>();
        for ( int i = 1; i <= n; i++ ) {
            list.addLast(i);
        }
        var it = next(list, list.listIterator() );
        while (list.size() > 1) {
            it = next(list, it);
            it.remove();
            it = next(list, it); // remove() call brings us to the previous element
        }
        return list.get(0);
    }

    @Test
    public void solution() {
        Assert.assertEquals( "test", 3, detectWinner(5));
        Assert.assertEquals( "answer 1", 1842613, detectWinner(3018458));
    }
}
