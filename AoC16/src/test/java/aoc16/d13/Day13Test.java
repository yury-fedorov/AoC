package aoc16.d13;

import org.javatuples.Pair;
import org.junit.Assert;
import org.junit.Test;

import java.util.*;

public class Day13Test {

    static boolean isWall( int x, int y, int favoriteNumber ) {
        int sum = x*x + 3*x + 2*x*y + y + y*y + favoriteNumber;
        int countOne = 0;
        for ( ; sum != 0; sum = sum >> 1 ) {
            countOne += ( sum & 1 ) != 0 ? 1 : 0;
        }
        return ( countOne % 2 ) == 1;
    }

    static Collection<Pair<Integer,Integer>> next( int x, int y, int favoriteNumber ) {
        final var result = new ArrayList<Pair<Integer,Integer>>();
        for ( int dx = -1; dx <= 1; dx++ ) {
            for ( int dy = -1; dy <= 1; dy++ ) {
                if ( dx == 0 && dy == 0 ) continue;
                if ( dx != 0 && dy != 0 ) continue;
                final var p0d = Pair.with( x + dx, y + dy );
                if ( p0d.getValue0() < 0 || p0d.getValue1() < 0 ) continue;
                if ( isWall( p0d.getValue0(), p0d.getValue1(), favoriteNumber ) ) continue;
                result.add( p0d );
            }
        }
        return result;
    }

    static int minDistance( final Pair<Integer,Integer> p0, final Pair<Integer,Integer> p1, final int favoriteNumber ) {
        if ( p0 == p1 ) return 0;
        var prevStep = new HashSet<Pair<Integer,Integer>>();
        final var path = new HashSet<Pair<Integer,Integer>>();
        prevStep.add(p0);
        path.add(p0);
        for ( int result = 1; true; result++ ) {
            final var nextToPrev = new HashSet<Pair<Integer,Integer>>();
            for ( final var psp : prevStep ) {
                for ( final var np : next( psp.getValue0(), psp.getValue1(), favoriteNumber ) ) {
                    if ( !path.contains( np ) )
                        nextToPrev.add( np );
                }
            }
            if ( nextToPrev.contains(p1) ) return result;
            path.addAll( nextToPrev );
            prevStep = nextToPrev;
        }
    }

    static int allWithinDistance(final Pair<Integer, Integer> p0, final int favoriteNumber, final int maxDistance ) {
        var prevStep = new HashSet<Pair<Integer,Integer>>();
        final var path = new HashSet<Pair<Integer,Integer>>();
        prevStep.add(p0);
        path.add(p0);
        for ( int result = 1; result <= maxDistance; result++ ) {
            final var nextToPrev = new HashSet<Pair<Integer,Integer>>();
            for ( final var psp : prevStep ) {
                for ( final var np : next( psp.getValue0(), psp.getValue1(), favoriteNumber ) ) {
                    if ( !path.contains( np ) )
                        nextToPrev.add( np );
                }
            }
            path.addAll( nextToPrev );
            prevStep = nextToPrev;
        }
        return path.size();
    }

    @Test
    public void solution() {
        final var P0 = Pair.with(1,1);
        Assert.assertEquals( "test", 11, minDistance( P0, Pair.with(7,4), 10) );
        final int FAVORITE_NUMBER = 1352;
        Assert.assertEquals( "answer 1", 90, minDistance( P0, Pair.with(31,39), FAVORITE_NUMBER ) );
        Assert.assertEquals( "answer 2", 135, allWithinDistance( P0, FAVORITE_NUMBER, 50 ) );
    }
}
