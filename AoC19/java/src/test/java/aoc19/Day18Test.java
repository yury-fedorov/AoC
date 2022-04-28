package aoc19;

import org.junit.Test;

import java.util.*;

import static org.junit.Assert.assertEquals;

public class Day18Test {

    static final char WALL = '#';
    static final char PATH = '.';
    static final char START = '@';
    static boolean isKey(char c) { return Character.isLowerCase(c); }
    static boolean isDoor(char c) { return Character.isUpperCase(c); }

    record Point( int x, int y ) {}

    @Test
    public void solution() {
        final var input = IOUtil.input("day18");
        final var maxY = input.size();
        final var map = new HashMap<Point,Character>();
        final var mapKey = new HashMap<Character, Point>();
        final var mapDoor = new HashMap<Character, Point>();
        final var walkable = new HashSet<Point>();
        Point start = null;
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
        var next = Set.of( start );
        var distance = 0;
        final var distanceMap = new HashMap<Point,Integer>();
        final var nextDoors = new HashMap<Character, Integer>();
        final var nextKeys = new HashMap<Character, Integer>();
        while ( !next.isEmpty() ) {
            final var next1 = new HashSet<Point>();
            for ( var n : next ) {
                boolean isNext = true;
                final var doorId = getIdByPoint(mapDoor, n);
                final var keyId = getIdByPoint(mapKey, n);
                if ( doorId.isPresent() ) {
                    // this is a door (we stop here)
                    nextDoors.put( doorId.get(), distance );
                    isNext = false; // we stop analyses here (at least for now)
                } else if ( keyId.isPresent() ) {
                    // this is a key
                    nextKeys.put( keyId.get(), distance );
                }
                if ( isNext ) {
                    distanceMap.put(n, distance);
                    final var n1 = next(n, walkable, distanceMap);
                    next1.addAll(n1);
                }
            }
            next = next1;
            distance++;
        }
        // How many steps is the shortest path that collects all of the keys?
        assertEquals("answer 1", -1, 0);
        assertEquals("answer 2", -2, 0);
    }

    static Optional<Character> getIdByPoint( Map<Character,Point> idPointMap, Point p) {
        return idPointMap.entrySet().stream()
                .filter( e -> e.getValue().equals( p ) )
                .map( e -> e.getKey() )
                .findFirst();
    }

    static List<Point> next( Point from, Set<Point> walkable, Map<Point,Integer> distance ) {
        var result = new ArrayList<Point>(4);
        result.add( new Point( from.x - 1, from.y ) );
        result.add( new Point( from.x + 1, from.y ) );
        result.add( new Point( from.x, from.y - 1 ) );
        result.add( new Point( from.x, from.y + 1 ) );
        result.retainAll( walkable );
        result.removeAll( distance.keySet() );
        return result;
    }
}
