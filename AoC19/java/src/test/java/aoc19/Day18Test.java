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
        final var calculator = new PathCalculator(
                Collections.unmodifiableSet(walkable),
                Collections.unmodifiableMap(mapKey),
                Collections.unmodifiableMap(mapDoor) );

        var answer1 = Integer.MAX_VALUE;
        final var ALL_KEYS = mapKey.size();
        var next = calculator.nextStep(Optional.empty(), start);
        while ( !next.isEmpty() ) {
            final var minDistance = next.stream().filter( s -> s.keys().size() == ALL_KEYS )
                    .mapToInt( s -> s.totalDistance() ).min();
            if ( minDistance.isPresent() ) {
                answer1 = minDistance.getAsInt();
                break; // TODO - not sure that we can stop here
            }
            var next1 = new ArrayList<Step>();
            for ( var s : next ) {
                final var s1 = calculator.nextStep( Optional.of(s), getPoint(s.event, mapKey, mapDoor ) );
                next1.addAll(s1);
            }
        }

        // How many steps is the shortest path that collects all of the keys?
        assertEquals("answer 1", -1, answer1);
        assertEquals("answer 2", -2, 0);
    }

    static Point getPoint( char event, Map<Character,Point> keys, Map<Character,Point> doors ) {
        return keys.getOrDefault( event, doors.get(event) );
    }

    // optimized in space, simple recursive processing (tree)
    record Step( Optional<Step> previous, char event, int distance ) {
        // boolean isKey() { return Day18Test.isKey(event); }
        int totalDistance() { return distance + (previous.isPresent() ? previous.get().totalDistance() : 0); }
        List<Character> keys() { return getList( c -> isKey(c) ); }
        List<Character> openedDoors() { return getList( c -> isDoor(c) ); }
        private List<Character> getList( Predicate<Character> isGood ) {
            var result = new ArrayList<Character>();
            var current = Optional.of( this );
            while ( current.isPresent() ) {
                if ( isGood.test( current.get().event ) ) result.add( current.get().event );
                current = current.get().previous;
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

    static List<Point> nextPoint( Point from, Set<Point> walkable, Map<Point,Integer> distance ) {
        var result = new ArrayList<Point>(4);
        result.add( new Point( from.x - 1, from.y ) );
        result.add( new Point( from.x + 1, from.y ) );
        result.add( new Point( from.x, from.y - 1 ) );
        result.add( new Point( from.x, from.y + 1 ) );
        result.retainAll( walkable );
        result.removeAll( distance.keySet() );
        return result;
    }

    record PathCalculator( Set<Point> walkable, Map<Character, Point> keys, Map<Character, Point> doors ) {
        List<Step> nextStep( Optional<Step> prev, Point start ) {
            final var doorsNow = new HashMap<>( doors );
            final var keysNow = new HashMap<>( keys );
            if ( prev.isPresent() ) {
                for ( var od : prev.get().openedDoors() ) {
                    doorsNow.remove(od);
                }
                for ( var k : prev.get().keys() ) {
                    keysNow.remove(k);
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
                    final var doorId = getIdByPoint(doorsNow, n);
                    final var keyId = getIdByPoint(keysNow, n);
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
                        final var n1 = nextPoint(n, walkable, distanceMap);
                        next1.addAll(n1);
                    }
                }
                next = next1;
                distance++;
            }

            final var result = new ArrayList<Step>();
            for ( var ke : nextKeys.entrySet() ) {
                result.add( new Step( prev, ke.getKey(), ke.getValue() ) );
            }

            if ( prev.isPresent() ) {
                // logic is we need to analyse all paths to next keys or doors for which we already have keys
                // on every option the analysis is repeated (so we need to keep the state (which keys collected,
                // which doors opened, where we stay, distance walked so far)
                var path = prev.get();
                var openableDoors = new ArrayList<Character>( path.keys().stream().map( k -> keyToDoor(k) ).toList() );
                openableDoors.removeAll( path.openedDoors() );
                openableDoors.retainAll( nextDoors.keySet() );
                // this is the set of doors to analyze

                for ( var od : openableDoors ) {
                    result.add( new Step( prev, od, nextDoors.get(od) ) );
                }
            }

            return result;
        }
    }
}
