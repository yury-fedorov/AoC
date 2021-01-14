package aoc16.d24;

import aoc16.common.IOUtil;
import aoc16.common.Point;
import org.javatuples.Pair;
import org.junit.Assert;
import org.junit.Test;

import java.util.*;
import java.util.function.Function;
import java.util.stream.Collectors;

public class Day24Test {

    final static char WALL = '#';
    final static char SPACE = '.';
    final static List<Point> SHIFTS
            = List.of( Point.with(1,0), Point.with(-1,0), Point.with(0,1), Point.with(0,-1) );

    static Point size( List<String> map ) { return Point.with( map.get(0).length(), map.size() ); }

    static char at( List<String> map, Point p ) { return map.get( p.y ).charAt( p.x ); }

    static Pair<Integer, Collection<Character>> realDistance( List<String> map, Point p0, Point p1 ) {
        final var beenPoints = new HashSet<Point>();
        final var passed = new HashSet<Character>();
        var frontLine = new HashSet<Point>();
        final var s = size(map);
        final Function<Point,Boolean> isIn = (p) -> p.x >= 0 && p.y >= 0 && p.x < s.x && p.y < s.y;
        frontLine.add( p0 );
        int d = 0;
        for ( ; !frontLine.isEmpty(); d++ ) {
            var next = new HashSet<Point>();
            for ( final var point : frontLine ) {
                if ( point.equals( p1 ) ) return Pair.with( d, passed );
                final char ch = at(map, point);
                Assert.assertNotEquals( "walls are not expected", ch, WALL );
                if ( ch != SPACE && !point.equals(p0) ) passed.add( ch );
                next.addAll( SHIFTS.stream()
                        .map( (sh) -> Point.add(point, sh) )
                        .filter( (p) -> isIn.apply(p) )
                        .filter( (p) -> !beenPoints.contains(p) )
                        .filter( (p) -> at(map, p) != WALL )
                        .collect(Collectors.toList()) );
            }
            beenPoints.addAll(frontLine);
            frontLine = next;
        }
        return Pair.with(d, passed);
    }

    public static Collection<String> paths( StringBuilder toGo, String passed ) {
        final var l = toGo.length();
        final boolean isLast = l == 1;
        final var result = new ArrayList<String>(l);
        for ( int index = 0; index < l; index++ ) {
            final Character ch = toGo.charAt(index);
            final String passed1 = passed + ch;
            if ( isLast ) result.add( passed1 );
            else {
                final var toGo1 = new StringBuilder( toGo );
                toGo1.deleteCharAt(index);
                result.addAll( paths( toGo1, passed1 ) );
            }
        }
        return result;
    }

    static Pair<Character,Character> key( char a, char b ) { return a < b ? Pair.with( a, b ) : Pair.with( b, a );  }

    @Test
    public void solution() {
        final var map = IOUtil.input("d24");
        final var s = size(map);
        final var m = new HashMap<Character, Point>();
        for ( var y = 0; y < s.y; y++ ) {
            final var l = map.get(y);
            for ( var x = 0; x < s.x; x++ ) {
                final var ch = l.charAt(x);
                if ( ch != WALL && ch != SPACE ) m.put( ch, Point.with(x,y) );
            }
        }
        final var manhattanDistances = new HashMap<Pair<Character,Character>,Integer>();
        for ( final var a : m.keySet() ) {
            for ( final var b : m.keySet() ) {
                if ( a >= b ) continue;
                manhattanDistances.put( Pair.with(a,b), Point.distance( m.get(a), m.get(b) ) );
            }
        }
        final var realDistance = new HashMap<Pair<Character,Character>, Pair<Integer,Collection<Character>> >();
        for ( final var k : manhattanDistances.keySet() ) {
            realDistance.put(k, realDistance(map, m.get( k.getValue0() ), m.get( k.getValue1() ) ) );
        }
        final var toGo = new StringBuilder( '0' );
        for ( final char ch : m.keySet() ) if ( ch != '0' ) toGo.append( ch );
        final var allPaths = paths( toGo, "0" );

        final int answer1 = allPaths.stream().mapToInt( (p) -> distance(realDistance, p) ).min().getAsInt();
        Assert.assertEquals( "answer 1", 490, answer1 );

        final int answer2 = allPaths.stream().mapToInt( (p) -> distance(realDistance, p + "0" ) ).min().getAsInt();
        Assert.assertEquals( "answer 2",  744, answer2 );
    }

    private int distance(HashMap<Pair<Character, Character>, Pair<Integer, Collection<Character>>> realDistance, String p) {
        int distance = 0;
        char prev = '?';
        for ( final var next : p.toCharArray() ) {
            if ( prev == '?' ) prev = next;
            else {
                distance += realDistance.get( key( prev, next ) ).getValue0();
                prev = next;
            }
        }
        return distance;
    }
}
