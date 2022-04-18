package aoc19;

import org.junit.Test;
import static org.junit.Assert.*;

public class Day24Test {

    // TODO - to cover with tests?
    record Point( int x, int y ) {
        public long getBiodiversityRating(int maxX) {
            var index = ( maxX * y ) + x + 1;
            return (long)Math.pow( index, 2 );
        }
    }

    @Test
    public void solution() {
        assertEquals( "answer 1", -1, 0 );
    }
}
