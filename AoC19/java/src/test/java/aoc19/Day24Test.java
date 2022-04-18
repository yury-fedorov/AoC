package aoc19;

import org.junit.Test;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;

import static org.junit.Assert.assertEquals;

public class Day24Test {

    public final static int SIZE = 5; // field dimension (x or y)

    public final static char BUG = '#';
    public final static char SPACE = '.'; // empty space

    record Point( int x, int y ) {
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

    static String mapToString(Map<Point,Character> map) {
        var result = new StringBuilder( SIZE * SIZE );
        for (int y = 0; y < SIZE; y++ ) {
            for (int x = 0; x < SIZE; x++ ) {
                result.append( map.get( new Point(x,y) ) );
            }
        }
        return result.toString();
    }

    static boolean isAdjacent( Point a, Point b ) {
        var dx = Math.abs(a.x - b.x);
        var dy = Math.abs(a.y - b.y);
        return (dx + dy) == 1;
    }

    static long countBugs( Map<Point,Character> map, Point point ) {
        return map.entrySet().stream()
                .filter( e -> e.getValue() == BUG && isAdjacent(point, e.getKey()) )
                .count();
    }

    @Test
    public void solution() {
        assertEquals( 1, new Point(0,0).getBiodiversityRating() );
        assertEquals( 2, new Point(1,0).getBiodiversityRating() );
        assertEquals( 4, new Point(2,0).getBiodiversityRating() );
        assertEquals( 32768, new Point(0,3).getBiodiversityRating() );

        var input = IOUtil.input("day24");
        var map = new HashMap<Point,Character>();
        for (int x = 0; x < SIZE; x++ ) {
            for (int y = 0; y < SIZE; y++ ) {
                map.put( new Point(x,y), input.get(y).charAt(x) );
            }
        }
        var history = new HashSet<String>();
        history.add(mapToString(map));
        while (true) {
            // adjust map
            var map1 = new HashMap<Point,Character>();
            for (int x = 0; x < SIZE; x++ ) {
                for (int y = 0; y < SIZE; y++ ) {
                    final var p = new Point(x,y);
                    final var bugs = countBugs(map, p);
                    final var isBug = map.get(p) == BUG;
                    final var isBug1 = isBug ? bugs == 1 : ( bugs == 1 || bugs == 2 );
                    map1.put(p, isBug1 ? BUG : SPACE );
   
                }
            }
            map = map1;
            var s = mapToString(map);
            var isAdded = history.add(s);
            if ( !isAdded ) break;
        }

        var rating = getBiodiversityRating(map);

        assertEquals( "answer 1", -1, rating );
    }
}
