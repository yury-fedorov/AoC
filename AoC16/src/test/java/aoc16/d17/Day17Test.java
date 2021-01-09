package aoc16.d17;

import aoc16.common.Config;
import aoc16.common.Md5Util;
import org.javatuples.Pair;
import org.junit.Assert;
import org.junit.Test;

import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Day17Test {

    enum Direction {
        UP('U', 0, -1, 0),
        DOWN('D', 0, 1, 1),
        LEFT('L', -1, 0, 2),
        RIGHT('R', 1, 0, 3);
        public final char _symbol;
        public final int _dx;
        public final int _dy;
        public final int _position;
        Direction( char symbol, int dx, int dy, int p ) {
            _symbol = symbol; _dx = dx; _dy = dy; _position = p;
        }
    }

    static boolean isOpen( char code ) { return code >= 'b' && code <= 'f'; }

    static Collection<Direction> openDoors(String text ) {
        final var hash = md5(text);
        return Stream.of(Direction.values()).filter( (d) -> isOpen( hash.charAt( d._position ) ) )
                .collect(Collectors.toList());
    }

    @org.jetbrains.annotations.NotNull
    static String md5(String text ) { return new Md5Util().md5(text).toLowerCase(); }

    static Optional<Pair<Integer,Integer>> move( Pair<Integer,Integer> p0, Direction d ) {
        final var p = Pair.with( p0.getValue0() + d._dx, p0.getValue1() + d._dy );
        if ( p.getValue0() < 0 || p.getValue1() < 0 || p.getValue0() > MAX_COOR || p.getValue1() > MAX_COOR )
            return Optional.empty();
        return Optional.of(p);
    }

    final static int MAX_COOR = 4 - 1;

    static List<String> allPaths(final String passcode ) {
        final var result = new LinkedList<String>();
        final var px = Pair.with(MAX_COOR,MAX_COOR);

        var prev = new LinkedList<Pair<String,Pair<Integer,Integer>>>();
        prev.add( Pair.with( "", Pair.with(0,0) ) );

        while ( !prev.isEmpty() ) {
            final var next = new LinkedList<Pair<String,Pair<Integer,Integer>>>();
            for ( final var start : prev ) {
                final var path = start.getValue0();
                final var p0 = start.getValue1();
                final var options = openDoors(passcode + path ).stream()
                        .map( (d) -> Pair.with( d, move( p0, d ) ) )
                        .filter( (p) -> p.getValue1().isPresent() )
                        .map( (p) -> Pair.with( p.getValue0()._symbol, p.getValue1().get() ) )
                        .collect(Collectors.toList());

                for ( final var p : options ) {
                    final var path1 = path + p.getValue0();
                    if ( p.getValue1().equals(px) ) result.add( path1 );
                    else next.add( Pair.with( path1, p.getValue1() ) );
                }
            }
            prev = next;
        }
        return result;
    }

    static Pair<String,Integer> answers( String passcode ) {
        final var all =  allPaths( passcode );
        return Pair.with(all.get(0), all.listIterator().next().length());
    }

    static String shortestPath( String passcode ) { return answers( passcode ).getValue0(); }

    @Test
    public void test() {
        if ( Config.isFast() ) return; // takes 5 seconds
        Assert.assertEquals( "DDRRRD", shortestPath("ihgpwlah") );
        Assert.assertEquals( "DDUDRLRRUDRD", shortestPath("kglvqrro") );
        Assert.assertEquals( "DRURDRUDDLLDLUURRDULRLDUUDDDRR", shortestPath("ulqzkmiv") );
    }

    @Test
    public void solution() {
        final var INPUT = "gdjjyniy";
        final var all =  allPaths( INPUT );
        String answer1 = all.get(0);
        int answer2 = all.get(0).length();
        for ( final var e : all ) {
            answer2 = Math.max( answer2, e.length() );
            if ( answer1.length() > e.length() ) answer1 = e;
        }
        Assert.assertEquals( "answer 1", "DUDDRLRRRD", answer1 );
        Assert.assertEquals( "answer 2", 578, answer2 );
    }
}
