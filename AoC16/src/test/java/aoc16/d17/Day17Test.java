package aoc16.d17;

import aoc16.common.Md5Util;
import org.javatuples.Pair;
import org.junit.Assert;
import org.junit.Test;

import java.util.Collection;
import java.util.LinkedList;
import java.util.Map;
import java.util.Optional;
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

    static String shortestPath( final String passcode ) {
        var p0 = Pair.with(0,0);
        final var px = Pair.with(MAX_COOR,MAX_COOR);

        final var next = new LinkedList<Pair<String,Pair<Integer,Integer>>>();

        String path = "";
        final var options = openDoors(passcode).stream()
                .map( (d) -> Pair.with( d, move( p0, d ) ) )
                .filter( (p) -> p.getValue1().isPresent() )
                .map( (p) -> Pair.with( p.getValue0()._symbol, p.getValue1().get() ) )
                .collect(Collectors.toList());
        final var or = options.stream().filter( (p) -> p.getValue1() == px )
                .map( (p) -> p.getValue0() ).findFirst();
        if (or.isPresent()) return path + or.get();

        return "";
    }

    @Test
    public void test() {
        Assert.assertEquals( "DDRRRD", shortestPath("ihgpwlah") );
        Assert.assertEquals( "DDUDRLRRUDRD", shortestPath("kglvqrro") );
        Assert.assertEquals( "DRURDRUDDLLDLUURRDULRLDUUDDDRR", shortestPath("ulqzkmiv") );
    }

    @Test
    public void solution() {
        final var INPUT = "gdjjyniy";
        Assert.assertEquals( "answer 1", null, shortestPath( INPUT ) );
    }
}
