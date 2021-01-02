package aoc16.d08;
import org.javatuples.Pair;
import org.junit.Test;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Collection;
import java.util.HashSet;
import java.util.Set;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

public class Day08Test {

    static Set<Pair<Integer,Integer>> newSet() { return new HashSet<Pair<Integer,Integer>>(); }

    static void print( final int width, final int height, Set<Pair<Integer,Integer>> screen ) {
        for ( int y = 0; y < height; y++ ) {
            for ( int x = 0; x < width; x++ ) {
                System.out.print( screen.contains( Pair.with(x,y) ) ? '#' : ' ' );
            }
            System.out.println();
        }
        System.out.println();
    }

    static Set<Pair<Integer,Integer>> generic( final int width, final int height, Collection<String> input ) {
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
                        screen.add( Pair.with( x, y ) );
                    }
                }
            } else {
                // common is shift
                final var ns = newSet();
                matcher = patternRow.matcher(l);
                if ( matcher.find() ) {
                    final var y = Integer.parseInt( matcher.group(1) );
                    final var n = Integer.parseInt( matcher.group(2) );
                    ns.addAll( screen.stream().filter( a -> a.getValue1() != y ).collect(Collectors.toList()) );
                    ns.addAll( screen.stream().filter( a -> a.getValue1() == y )
                            .map( a -> Pair.with( ( a.getValue0() + n ) % width, a.getValue1() ) )
                            .collect(Collectors.toList()));
                } else {
                    matcher = patternCol.matcher(l);
                    if (matcher.find()) {
                        final var x = Integer.parseInt(matcher.group(1));
                        final var n = Integer.parseInt(matcher.group(2));
                        ns.addAll( screen.stream().filter( a -> a.getValue0() != x ).collect(Collectors.toList()) );
                        ns.addAll( screen.stream().filter( a -> a.getValue0() == x )
                                .map( a -> Pair.with( a.getValue0(), ( a.getValue1() + n ) % height ) )
                                .collect(Collectors.toList()));
                    } else {
                        System.err.println(l);
                        assertTrue( false );
                    }
                }
                screen = ns;
            }
        }
        return screen;
    }

    @Test
    public void sample() throws IOException {
        final var url = getClass().getClassLoader().getResource( "d08/sample.txt" );
        final var input = Files.readAllLines( Path.of(url.getPath()) );
        assertEquals("test", 6, generic( 7, 3, input ).size() );
    }

    @Test
    public void solution() throws IOException {
        final var url = getClass().getClassLoader().getResource( "d08/input.txt" );
        final var input = Files.readAllLines( Path.of(url.getPath()) );
        final var screen = generic( 50, 6, input );
        assertEquals("answer 1", 123, screen.size() );
        print( 50, 6, screen ); // answer 2 is printed: AFBUPZBJPS
    }
}
