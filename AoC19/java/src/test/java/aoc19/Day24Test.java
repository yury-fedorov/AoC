package aoc19;

import org.junit.Test;
import static org.junit.Assert.assertEquals;

public class Day24Test {

    public final static int SIZE = 5; // field dimension (x or y)

    record Point( int x, int y ) {
        public long getBiodiversityRating(int maxX) {
            var index = ( maxX * y ) + x;
            return (long)Math.pow( 2, index );
        }
        public long getBiodiversityRating() {
            return getBiodiversityRating(SIZE);
        }
    }

    @Test
    public void solution() {
        assertEquals( 1, new Point(0,0).getBiodiversityRating() );
        assertEquals( 2, new Point(1,0).getBiodiversityRating() );
        assertEquals( 4, new Point(2,0).getBiodiversityRating() );
        assertEquals( 32768, new Point(0,3).getBiodiversityRating() );
        assertEquals( "answer 1", -1, 0 );
    }
}
