package aoc16.d24;

import aoc16.common.IOUtil;
import org.javatuples.Pair;
import org.junit.Assert;
import org.junit.Test;

import java.util.HashMap;

public class Day24Test {

    final static char WALL = '#';
    final static char SPACE = '.';

    @Test
    public void solution() {
        final var input = IOUtil.input("d24");
        final var sizeY = input.size();
        final var sizeX = input.get(0).length();
        final var m = new HashMap<Character, Pair<Integer,Integer>>();
        for ( var y = 0; y < sizeY; y++ ) {
            final var l = input.get(y);
            for ( var x = 0; x < sizeX; x++ ) {
                final var ch = l.charAt(x);
                if ( ch != WALL && ch != SPACE ) m.put( ch, Pair.with(x,y) );
            }
        }
        Assert.fail("to implement");
    }
}
