package aoc16.d15;

import aoc16.common.IOUtil;
import org.javatuples.Pair;
import org.junit.Assert;
import org.junit.Test;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.regex.Pattern;

public class Day15Test {

    static boolean fallsThroughSlot( final int t, final int positions, final int position0 ) {
        final var position = ( position0 + t ) % positions;
        return position == 0;
    }

    static Optional<Integer> whereBouncedAway( final int t0, List<Pair<Integer,Integer>> disks ) {
        for ( int i = 0; i < disks.size(); i++ ) {
            final var disk = disks.get(i);
            if ( !fallsThroughSlot( t0 + i + 1, disk.getValue0(), disk.getValue1() ) ) return Optional.of(i);
        }
        return Optional.empty();
    }

    @Test
    public void test() {
        final var disks = new ArrayList<Pair<Integer,Integer>>();
        disks.add( Pair.with( 5, 4 ) );
        disks.add( Pair.with( 2, 1 ) );
        Assert.assertEquals( 5, getT(disks) );
    }

    @Test
    public void solution() {
        final var input = IOUtil.input("d15");
        final var pattern = Pattern.compile(
                "^Disc #(\\d+) has (\\d+) positions; at time=0, it is at position (\\d+).$");
        final var disks = new ArrayList<Pair<Integer,Integer>>();
        for ( final var l : input ) {
            final var m = pattern.matcher(l);
            Assert.assertTrue( "Unexpected format: " + l, m.find() );
            disks.add( Pair.with( Integer.parseInt( m.group(2) ), Integer.parseInt( m.group(3) ) ) );
        }
        Assert.assertEquals( "answer 1", 148737, getT(disks) );
        disks.add( Pair.with(11,0) );
        Assert.assertEquals( "answer 2", 2353212, getT(disks) );
    }

    private int getT(ArrayList<Pair<Integer, Integer>> disks) {
        for (  int t = 0; true; t++ ) { if ( whereBouncedAway( t, disks).isEmpty() ) return t; }
    }
}
