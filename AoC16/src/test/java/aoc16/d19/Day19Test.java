package aoc16.d19;

import aoc16.common.Config;
import org.junit.Assert;
import org.junit.Test;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.ListIterator;

public class Day19Test {

    static ListIterator<Integer> next( LinkedList<Integer> list, ListIterator<Integer> it ) {
        if ( !it.hasNext() ) it = list.listIterator();
        it.next();
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
        return list.peek();
    }

    static int detectWinner2( final int initialSize ) {
        final LinkedList<Integer> list = init(initialSize);
        for (  int position = 0; true; ) {
            final int n = list.size();
            if ( n == 1 ) return list.peek();
            final int half = ( n >> 1 ); // divide by 2
            final int outIndex = ( position + half ) % n;
            list.remove( outIndex );
            if ( outIndex > position ) position++;
        }
    }

    static int detectWinner21( final int initialSize ) {
        final List<Integer> list = new ArrayList<>( init(initialSize) );
        for (  int position = 0; true; ) {
            final int n = list.size();
            if ( n == 1 ) return list.get(0);
            if ( ( n % 10000 ) == 0 ) System.out.println( n );
            final int half = ( n >> 1 ); // divide by 2
            final int outIndex = ( position + half ) % n;
            list.remove( outIndex );
            if ( outIndex > position ) position++;
        }
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
        Assert.assertEquals( "test 2", 2, detectWinner21(5));
        if (Config.isFast() ) return; // 1 h 8 minutess 16 seconds
        Assert.assertEquals( "answer 2", -1, detectWinner21(INPUT)); // 259778 - too low
    }
}
