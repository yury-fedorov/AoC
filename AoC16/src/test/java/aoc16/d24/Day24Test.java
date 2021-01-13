package aoc16.d24;

import aoc16.common.IOUtil;
import aoc16.common.Point;
import org.javatuples.Pair;
import org.junit.Assert;
import org.junit.Test;

import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;

public class Day24Test {

    final static char WALL = '#';
    final static char SPACE = '.';

    static Pair<Integer, Collection<Character>> realDistance( List<String> map, Point p0, Point p1 ) {
        final var beenPoints = new HashSet<Point>();
        final var passed = new HashSet<Character>();
        var frontLine = new HashSet<Point>();
        beenPoints.add( p0 );
        frontLine.add( p0 );
        int d = 0;
        for ( ; !frontLine.isEmpty(); d++ ) {
            for ( final var p : frontLine ) {

            }
            var next = new HashSet<Point>();
        }
        return Pair.with(d, passed);
    }

    @Test
    public void solution() {
        final var map = IOUtil.input("d24");
        final var sizeY = map.size();
        final var sizeX = map.get(0).length();
        final var m = new HashMap<Character, Point>();
        for ( var y = 0; y < sizeY; y++ ) {
            final var l = map.get(y);
            for ( var x = 0; x < sizeX; x++ ) {
                final var ch = l.charAt(x);
                if ( ch != WALL && ch != SPACE ) m.put( ch, Point.with(x,y) );
            }
        }
        final var manhattanDistances = new HashMap<Pair<Character,Character>,Integer>();
        for ( final var a : m.keySet() ) {
            for ( final var b : m.keySet() ) {
                if ( a >= b ) continue;
                manhattanDistances.put( Pair.with(a,b), Point.distance( m.get(a), m.get(b) ) );
            }
        }
        Assert.fail("to implement");
    }
}
