package aoc16.d18;

import aoc16.common.IOUtil;
import org.junit.Assert;
import org.junit.Test;

import java.util.LinkedList;

public class Day18Test {

    final static char SAFE = '.';
    final static char TRAP = '^';

    static boolean isTrap( int index, String prev ) {
        final char c = prev.charAt(index);
        final char l = index > 0 ? prev.charAt(index - 1) : SAFE;
        final char r = index < (prev.length() - 1) ? prev.charAt(index + 1) : SAFE;
        if ( l == TRAP && c == TRAP && r == SAFE ) return true;
        if ( c == TRAP && r == TRAP && l == SAFE ) return true;
        if ( l == TRAP && c == SAFE && r == SAFE ) return true;
        if ( r == TRAP && c == SAFE && l == SAFE ) return true;
        return false;
    }

    static long countSafe( final int rows, String firstRow ) {
        final var input = new LinkedList<String>();
        input.add(firstRow);
        for ( var prev = firstRow; input.size() < rows; ) {
            final var l = prev.length();
            final var r = new char[l];
            for ( int i = 0; i < l; i++ ) {
                r[i] = isTrap(i, prev) ? TRAP : SAFE;
            }
            prev = new String(r);
            input.add(prev);
        }
        return input.parallelStream().mapToLong( (s) -> s.chars().filter( (c) -> c == SAFE ).count() ).sum();
    }

    @Test
    public void solution() {
        final var firstRow = IOUtil.input("d18").get(0);
        Assert.assertEquals( "answer 1", 1982, countSafe(40, firstRow ) );
        Assert.assertEquals( "answer 2", -1, countSafe(400000, firstRow ) );
    }
}