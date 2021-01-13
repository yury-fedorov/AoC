package aoc16.d22;

import aoc16.common.IOUtil;
import org.javatuples.Pair;
import org.junit.Assert;
import org.junit.Test;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
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

        final var empty = new ArrayList<Pair<Integer,Integer>>(); // empty node(s)
        final var full = new LinkedList<Pair<Integer,Integer>>(); // "full" nodes
        final boolean toPrint = false; // visualisation map
        for ( int y = 0; y <= maxY; y++ ) {
            for ( int x = 0; x <= maxX; x++ ) {
                final var a = Pair.with(x,y);
                final var n = grid.get( a );
                final boolean isEmpty = ( used(n) / (double)size(n) ) < 0.1;
                final boolean isFull = used(n) > 100;
                if ( isEmpty ) empty.add( a );
                else if ( isFull ) full.add(a);
                // if (toPrint) System.out.print( used(n) + "/" + size(n) + "=" + ( used(n) * 100 / size(n) ) + " \t" );
                if (toPrint) System.out.print( isEmpty ? '_' : isFull ? '#' : '.' );
                System.out.print( ' ' );
            }
            if (toPrint) System.out.println();
        }
        // XXX solved analytically looking at the map
        final var iep = empty.get(0);
        int distance = iep.getValue0() + iep.getValue1() + maxX; // to arrive to initial point
        distance += 5 * ( maxX - 1 );
        Assert.assertEquals( "answer 2", 213, distance );
    }
}
