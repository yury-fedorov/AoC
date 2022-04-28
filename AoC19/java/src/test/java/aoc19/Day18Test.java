package aoc19;

import org.junit.Test;

import java.util.*;
import java.util.function.Predicate;

import static org.junit.Assert.assertEquals;

public class Day18Test {

    static final char WALL = '#';
    static final char PATH = '.';
    static final char START = '@';
    static boolean isKey(char c) { return Character.isLowerCase(c); }
    static boolean isDoor(char c) { return Character.isUpperCase(c); }
    static char keyToDoor(char key) { return Character.toUpperCase(key); }

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
        var path = new PathStep(null, ' ', 0 );
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
        // logic is we need to analyse all paths to next keys or doors for which we already have keys
        // on every option the analysis is repeated (so we need to keep the state (which keys collected,
        // which doors opened, where we stay, distance walked so far)
        var openableDoors = path.keys().stream().map( k -> keyToDoor(k) ).toList();
        openableDoors.removeAll( path.openedDoors() );
        openableDoors.retainAll( nextDoors.keySet() );
        // this is the set of doors to analyze

        // How many steps is the shortest path that collects all of the keys?
        assertEquals("answer 1", -1, 0);
        assertEquals("answer 2", -2, 0);
    }

    /* TODO - remove
    // this is enough to keep the history of a given path
    // we add a key once it is gathered
    // we add a door once it is opened
    record Path ( List<Character> sequence, int distance ) {
        List<Character> keys() { return sequence.stream().filter( c -> isKey(c) ).toList(); }
    }
     */

    // optimized in space, simple recursive processing (tree)
    record PathStep( PathStep previous, char event, int distance ) {
        // boolean isKey() { return Day18Test.isKey(event); }
        int totalDistance() {
            int result = 0;
            var current = this;
            while ( current != null ) {
                result += current.distance;
                current = current.previous;
            }
            return result;
        }
        List<Character> keys() { return getList( c -> isKey(c) ); }
        List<Character> openedDoors() { return getList( c -> isDoor(c) ); }
        private List<Character> getList( Predicate<Character> isGood ) {
            var result = new ArrayList<Character>();
            var current = this;
            while ( current != null ) {
                if ( isGood.test( current.event ) ) result.add( current.event );
                current = current.previous;
            }
            return result;
        }
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

    static Set<Character>
}
