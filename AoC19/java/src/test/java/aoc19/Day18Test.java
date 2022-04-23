package aoc19;

import org.junit.Ignore;
import org.junit.Test;

import java.util.HashMap;
import java.util.HashSet;

import static org.junit.Assert.assertEquals;

public class Day18Test {

    static final char WALL = '#';
    static final char PATH = '.';
    static final char START = '@';
    static boolean isKey(char c) { return Character.isLowerCase(c); }
    static boolean isDoor(char c) { return Character.isUpperCase(c); }

    record Point( int x, int y ) {}

    @Ignore( "to be implemented" ) // TODO
    @Test
    public void solution() {
        final var input = IOUtil.input("day18");
        final var maxY = input.size();
        final var map = new HashMap<Point,Character>();
        final var mapKey = new HashMap<Character, Point>();
        final var mapDoor = new HashMap<Character, Point>();
        final var walkable = new HashSet<Point>();
        Point start;
        for ( int y = 0; y < maxY; y++ ) {
            final var row = input.get(y);
            final var maxX = row.length();
            for ( int x = 0; x < maxX; x++ ) {
                final var p = new Point(x,y);
                final var m = row.charAt(x);
                boolean isWalkable = false;
                if ( m == START )  { start = p; isWalkable = true; }
                else if ( isDoor(m) ) { mapDoor.put(m, p); isWalkable = true; }
                else if ( isKey(m) ) { mapKey.put(m,p); isWalkable = true; }
                else if ( m == PATH ) { isWalkable = true; }
                if ( isWalkable ) walkable.add(p);
                map.put( p, m );
            }
        }
        // How many steps is the shortest path that collects all of the keys?
        assertEquals("answer 1", -1, 0);
        assertEquals("answer 2", -2, 0);
    }
}
