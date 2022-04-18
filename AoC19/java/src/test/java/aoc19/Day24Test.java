package aoc19;

import org.junit.Test;

import java.util.*;
import java.util.function.Predicate;

import static org.junit.Assert.assertEquals;

public class Day24Test {

    public final static int SIZE = 5; // field dimension (x or y)

    public final static char BUG = '#';
    public final static char SPACE = '.'; // empty space

    // level is required only for the part 2
    record Point( int x, int y, int level ) {
        public long getBiodiversityRating() {
            var index = ( SIZE * y ) + x;
            return (long)Math.pow( 2, index );
        }
    }

    static long getBiodiversityRating( Map<Point,Character> map ) {
        return map.entrySet().stream()
                .filter( e -> e.getValue() == BUG )
                .mapToLong( e -> e.getKey().getBiodiversityRating() )
                .sum();
    }
    static boolean isAdjacent( Point a, Point b ) {
        var dx = Math.abs(a.x - b.x);
        var dy = Math.abs(a.y - b.y);
        return (dx + dy) == 1;
    }

    public static boolean isCenter( Point p ) {
        return p.x == 2 && p.y == 2;
    }

    static List<Point> getAdjacent(Map<Point,Character> map, Point point) {
        final var result = new ArrayList<Point>();
        final var sameLevel = map.keySet().stream()
                .filter( p -> isAdjacent(point, p) ).toList();
        result.addAll(sameLevel.stream().filter( p -> !isCenter(p)).toList());
        // TODO to implement
        return result;
    }

    static long countBugs( Map<Point,Character> map, Point point, boolean isPart1 ) {
        var list = isPart1 ? List.of() : getAdjacent(map, point);
        Predicate<Point> isAdj = isPart1 ? p -> isAdjacent(point, p) : p -> list.contains(p);
        return map.entrySet().stream()
                .filter( e -> e.getValue() == BUG && isAdj.test(e.getKey()) )
                .count();
    }

    static long countBugs( Map<Point,Character> map ) {
        return map.entrySet().stream().filter( e -> e.getValue() == BUG ).count();
    }

    static HashMap<Point,Character> lifeCircle( Map<Point,Character> map, boolean isPart1 ) {
        var map1 = new HashMap<Point,Character>();
        // note they grow slower then one level per iteration
        final var minLevel = map.keySet().stream().mapToInt(p -> p.level).min().getAsInt() - 1;
        final var maxLevel = map.keySet().stream().mapToInt(p -> p.level).min().getAsInt() + 1;

        for ( var level = minLevel; level <= maxLevel; level++ ) {
            if ( isPart1 && level != 0 ) continue;
            for (int x = 0; x < SIZE; x++ ) {
                for (int y = 0; y < SIZE; y++ ) {
                    final var p = new Point(x,y,level);
                    if ( !isPart1 && isCenter(p) ) continue; // point is not defined in part 2
                    final var bugs = countBugs(map, p, isPart1);
                    final var isBug = map.getOrDefault(p, SPACE) == BUG;
                    final var isBug1 = isBug ? bugs == 1 : ( bugs == 1 || bugs == 2 );
                    map1.put(p, isBug1 ? BUG : SPACE );
                }
            }
        }
        return map1;
    }

    @Test
    public void solution() {
        assertEquals( 1, new Point(0,0,0).getBiodiversityRating() );
        assertEquals( 2, new Point(1,0,0).getBiodiversityRating() );
        assertEquals( 4, new Point(2,0,0).getBiodiversityRating() );
        assertEquals( 32768, new Point(0,3, 0).getBiodiversityRating() );

        final var input = IOUtil.input("day24");
        var map = new HashMap<Point,Character>();
        for (int x = 0; x < SIZE; x++ ) {
            for (int y = 0; y < SIZE; y++ ) {
                map.put( new Point(x,y,0), input.get(y).charAt(x) );
            }
        }
        var originalMap = (HashMap<Point,Character>)map.clone();
        final var history = new HashSet<Long>();
        history.add(getBiodiversityRating(map));
        while (true) {
            map = lifeCircle(map, true);
            final var hash = getBiodiversityRating(map);
            final var isAdded = history.add(hash);
            if ( !isAdded ) break;
        }
        final var rating = getBiodiversityRating(map);
        assertEquals( "answer 1", 18407158, rating );

        map = originalMap;
        for ( int t = 0; t < 200; t++ ) {
            map = lifeCircle(map, false);
        }
        assertEquals( "answer 2", -2, countBugs(map) );
    }
}