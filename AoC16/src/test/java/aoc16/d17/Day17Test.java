package aoc16.d17;

import aoc16.common.Config;
import aoc16.common.Md5Util;
import aoc16.common.Point;
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
    
    static String md5(String text ) { return new Md5Util().md5(text).toLowerCase(); }

    static Optional<Point> move(Point p0, Direction d ) {
        final var p = Point.with( p0.x() + d._dx, p0.y() + d._dy );
        if ( p.x() < 0 || p.y() < 0 || p.x() > MAX_COOR || p.y() > MAX_COOR )
            return Optional.empty();
        return Optional.of(p);
    }

    final static int MAX_COOR = 4 - 1;

    static record PathPoint(String path, Point point ) {}
    static record DirPoint(Direction direction, Optional<Point> point ) {}

    static List<String> allPaths(final String passcode ) {
        final var result = new LinkedList<String>();
        final var px = Point.with(MAX_COOR,MAX_COOR);

        var prev = new LinkedList<PathPoint>();
        prev.add( new PathPoint( "", Point.with(0,0) ) );

        while ( !prev.isEmpty() ) {
            final var next = new LinkedList<PathPoint>();
            for ( final var start : prev ) {
                final var path = start.path();
                final var p0 = start.point();
                final var options = openDoors(passcode + path ).stream()
                        .map( (d) -> new DirPoint( d, move( p0, d ) ) )
                        .filter( (p) -> p.point().isPresent() )
                        .map( (p) -> new PathPoint( ( (Character)( p.direction()._symbol) ).toString(), p.point().get() ) )
                        .collect(Collectors.toList());

                for ( final var p : options ) {
                    final var path1 = path + p.path();
                    if ( p.point().equals(px) ) result.add( path1 );
                    else next.add( new PathPoint( path1, p.point() ) );
                }
            }
            prev = next;
        }
        return result;
    }

    static record PathLength( String path, int length ) {}

    static PathLength answers( String passcode ) {
        final var all =  allPaths( passcode );
        return new PathLength(all.get(0), all.get(all.size()-1).length());
    }

    static String shortestPath( String passcode ) { return answers( passcode ).path(); }

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
        final var a = answers(INPUT);
        Assert.assertEquals( "answer 1", "DUDDRLRRRD", a.path() );
        Assert.assertEquals( "answer 2", 578, a.length() );
    }
}
