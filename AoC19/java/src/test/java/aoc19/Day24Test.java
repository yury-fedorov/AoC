package aoc19;

import org.junit.Test;

import java.util.HashMap;
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

        var rating = getBiodiversityRating(map);

        assertEquals( "answer 1", -1, rating );
    }
}
