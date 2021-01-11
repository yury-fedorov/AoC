package aoc16.d22;

import aoc16.common.IOUtil;
import org.javatuples.Pair;
import org.junit.Assert;
import org.junit.Test;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.regex.Pattern;

public class Day22Test {

    static int size( Pair<Integer,Integer> node ) { return node.getValue0(); }
    static int used( Pair<Integer,Integer> node ) { return node.getValue1(); }
    static int available( Pair<Integer,Integer> node ) { return node.getValue0() - node.getValue1(); }

    @Test
    public void solution() {
        final var input = IOUtil.input("d22");
        // /dev/grid/node-x0-y0     85T   67T    18T   78%
        final var pattern = Pattern.compile( "^/dev/grid/node-x(\\d+)-y(\\d+)\\s+(\\d+)T\\s+(\\d+)T\\s+(\\d+)T\\s+(\\d+)%$" );
        // TODO
        final var grid = new HashMap<Pair<Integer,Integer>, Pair<Integer,Integer>>();
        for ( final var l : input ) {
            final var m = pattern.matcher(l);
            if ( m.find() ) {
                grid.put( Pair.with( Integer.parseInt(m.group(1)), Integer.parseInt(m.group(2)) ), // x, y
                        Pair.with( Integer.parseInt(m.group(3)), Integer.parseInt(m.group(4)) ) ); // Size  Used
            } else System.out.println( "Bad format: " + l );
        }
        Assert.assertEquals( input.size() - 2, grid.size() );
        final var maxX = grid.keySet().stream().mapToInt( (p) -> p.getValue0() ).max().getAsInt();
        final var maxY = grid.keySet().stream().mapToInt( (p) -> p.getValue1() ).max().getAsInt();
        final var answer1 = new LinkedList<Pair< Pair<Integer,Integer>, Pair<Integer,Integer> >>(); // pair of nodes A->B
        for ( final var nodeA : grid.entrySet() ) {
            for ( final var nodeB : grid.entrySet() ) {
                if ( nodeA.getKey().equals( nodeB.getKey() ) ) continue;
                final var na = nodeA.getValue();
                final var nb = nodeB.getValue();
                if ( used(na) > 0 && used(na) <= available(nb) ) answer1.add( Pair.with(na,nb) );
            }
        }
        Assert.assertEquals( "answer 1", 924, answer1.size() );
        // important points:
        // 1) empty node(s)
        // 2) "full" nodes
        // 3) visualisation map
        Assert.fail( "no answer 2 yet" );
    }
}
