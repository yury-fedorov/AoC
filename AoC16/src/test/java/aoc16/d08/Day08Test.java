package aoc16.d08;

import aoc16.common.IOUtil;
import aoc16.common.Point;
import org.junit.Test;

import java.util.Collection;
import java.util.HashSet;
import java.util.Set;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

public class Day08Test {

    static Set<Point> newSet() { return new HashSet<>(); }

    static void print( final int width, final int height, Set<Point> screen ) {
        for ( int y = 0; y < height; y++ ) {
            for ( int x = 0; x < width; x++ ) {
                System.out.print( screen.contains( Point.with(x,y) ) ? '#' : ' ' );
            }
            System.out.println();
        }
        System.out.println();
    }

    static Set<Point> generic( final int width, final int height, Collection<String> input ) {
        var screen = newSet(); // only on are there
        final var patternRect = Pattern.compile("^rect (\\d+)x(\\d+)$");
        final var patternRow = Pattern.compile("^rotate row y=(\\d+) by (\\d+)$");
        final var patternCol = Pattern.compile("^rotate column x=(\\d+) by (\\d+)$");
        for ( final var l : input ) {
            var matcher = patternRect.matcher(l);
            if ( matcher.find() ) {
                final var a = Integer.parseInt( matcher.group(1) ); // wide
                final var b = Integer.parseInt( matcher.group(2) ); // tall
                for ( int x = 0; x < a; x++ ) {
                    for ( int y = 0; y < b; y++ ) {
                        screen.add( Point.with( x, y ) );
                    }
                }
            } else {
                // common is shift
                final var ns = newSet();
                matcher = patternRow.matcher(l);
                if ( matcher.find() ) {
                    final var y = Integer.parseInt( matcher.group(1) );
                    final var n = Integer.parseInt( matcher.group(2) );
                    ns.addAll( screen.stream().filter( a -> a.y != y ).collect(Collectors.toList()) );
                    ns.addAll( screen.stream().filter( a -> a.y == y )
                            .map( a -> Point.with( ( a.x + n ) % width, a.y ) )
                            .collect(Collectors.toList()));
                } else {
                    matcher = patternCol.matcher(l);
                    if (matcher.find()) {
                        final var x = Integer.parseInt(matcher.group(1));
                        final var n = Integer.parseInt(matcher.group(2));
                        ns.addAll( screen.stream().filter( a -> a.x != x ).collect(Collectors.toList()) );
                        ns.addAll( screen.stream().filter( a -> a.x == x )
                                .map( a -> Point.with( a.x, ( a.y + n ) % height ) )
                                .collect(Collectors.toList()));
                    } else {
                        System.err.println(l);
                        fail();
                    }
                }
                screen = ns;
            }
        }
        return screen;
    }

    @Test
    public void sample() {
        final var input = IOUtil.inputByPath( "d08/sample.txt" );
        assertEquals("test", 6, generic( 7, 3, input ).size() );
    }

    @Test
    public void solution() {
        final var input = IOUtil.input("d08");
        final var screen = generic( 50, 6, input );
        assertEquals("answer 1", 123, screen.size() );
        print( 50, 6, screen ); // answer 2 is printed: AFBUPZBJPS
    }
}