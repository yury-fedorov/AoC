package aoc19;

import org.junit.Test;

import java.util.*;
import java.util.function.Predicate;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import static org.junit.Assert.assertEquals;

public class Day24Test {

    public final static int SIZE = 5; // field dimension (x or y)

    public final static char BUG = '#';
    public final static char SPACE = '.'; // empty space

    // level is required only for the part 2
    record Point( int x, int y, int level ) {
        public long getBiodiversityRating() {
            var index = ( SIZE * y ) + x;
            return (long)Math.pow( 2, index );
        }
    }

    static long getBiodiversityRating( Map<Point,Character> map ) {
        return map.entrySet().stream()
                .filter( e -> e.getValue() == BUG )
                .mapToLong( e -> e.getKey().getBiodiversityRating() )
                .sum();
    }
    static boolean isAdjacent( Point a, Point b ) {
        var dx = Math.abs(a.x - b.x);
        var dy = Math.abs(a.y - b.y);
        return (dx + dy) == 1;
    }

    public static boolean isCenter( Point p ) {
        return p.x == 2 && p.y == 2;
    }

    static List<Point> border(int level, boolean isXFixed, int fixed ) {
        final var result = new ArrayList<Point>(SIZE);
        for ( int v = 0; v < SIZE; v++ ) {
            result.add( isXFixed ? new Point(fixed, v, level) : new Point(v, fixed, level) );
        }
        return result;
    }

    static List<Point> getAdjacent(Map<Point,Character> map, Point point) {
        final var result = new ArrayList<Point>();
        final var sameLevel = map.keySet().stream()
                .filter( p -> isAdjacent(point, p) ).toList();
        result.addAll(sameLevel.stream()
                .filter( p -> !isCenter(p))
                .map( p -> new Point( p.x, p.y, point.level ) )
                .toList());
        final var isCenterGate = sameLevel.size() > result.size(); // level in +1
        final var isBorderGate = sameLevel.size() < 4; // level out -1
        if ( isBorderGate ) {
            final var levelOut = point.level - 1;
            /*
            01234
            1.x.
            2x0x
            3.x.
            4
             */
            if ( point.x == 0 ) result.add( new Point( 1, 2, levelOut  ) );
            if ( point.y == 0 ) result.add( new Point( 2, 1, levelOut  ) );
            if ( point.x == 4 ) result.add( new Point( 3, 2, levelOut  ) );
            if ( point.y == 4 ) result.add( new Point( 2, 3, levelOut  ) );
        } else if ( isCenterGate ) {
            final var levelIn = point.level + 1;
            final var X_FIXED = true;
            final var Y_FIXED = false;
            if ( point.x == 1 ) {
                // left from center - right border of inner map
                result.addAll( border(levelIn, X_FIXED, 0 ) );
            } else if ( point.x == 3 ) {
                result.addAll( border(levelIn, X_FIXED, 4 ) );
            } else if ( point.y == 1 ) {
                result.addAll( border(levelIn, Y_FIXED, 0 ) );
            } else if ( point.y == 3 ) {
                result.addAll( border(levelIn, Y_FIXED, 4 ) );
            }
        }
        return result;
    }

    static long countBugs( Map<Point,Character> map, Point point, boolean isPart1 ) {
        var list = isPart1 ? List.of() : getAdjacent(map, point);
        Predicate<Point> isAdj = isPart1 ? p -> isAdjacent(point, p) : p -> list.contains(p);
        return map.entrySet().stream()
                .filter( e -> e.getValue() == BUG && isAdj.test(e.getKey()) )
                .count();
    }

    static long countBugs( Map<Point,Character> map ) {
        return map.entrySet().stream().filter( e -> e.getValue() == BUG ).count();
    }

    static IntStream mapToLevel(Map<Point,Character> map) {
        return map.entrySet().stream().filter( e -> e.getValue() == BUG ).mapToInt( e -> e.getKey().level);
    }

    record MinMax(int min, int max) {}

    static MinMax getMinMaxLevel(Map<Point,Character> map) {
        final var minLevel = mapToLevel( map ).min().orElse(0);
        final var maxLevel = mapToLevel( map ).max().orElse(0);
        return new MinMax( minLevel, maxLevel );
    }

    static String formatMap(Map<Point,Character> map, int level) {
        var result = new StringBuilder( SIZE * SIZE );
        for (int y = 0; y < SIZE; y++ ) {
            for (int x = 0; x < SIZE; x++ ) {
                result.append( map.get( new Point(x,y,level) ) );
            }
            result.append( System.lineSeparator() );
        }
        return result.toString();
    }

    static HashMap<Point,Character> lifeCircle( Map<Point,Character> map, boolean isPart1 ) {
        var map1 = new HashMap<Point,Character>();
        var minMaxLevel = getMinMaxLevel(map);
        var minLevel = minMaxLevel.min - 1;
        var maxLevel = minMaxLevel.max + 1;
        // note they grow slower then one level per iteration
        for ( var level = minLevel; level <= maxLevel; level++ ) {
            if ( isPart1 && level != 0 ) continue;
            for (int x = 0; x < SIZE; x++ ) {
                for (int y = 0; y < SIZE; y++ ) {
                    final var p = new Point(x,y,level);
                    if ( !isPart1 && isCenter(p) ) continue; // point is not defined in part 2
                    final var bugs = countBugs(map, p, isPart1);
                    final var isBug = map.getOrDefault(p, SPACE) == BUG;
                    final var isBug1 = isBug ? bugs == 1 : ( bugs == 1 || bugs == 2 );
                    map1.put(p, isBug1 ? BUG : SPACE );
                }
            }
        }
        return map1;
    }

    @Test
    public void demoPart2() {
        var map = loadMap("day24-sample");
        var loi = formatMap(map,0);
        for ( int t = 0; t < 10; t++ ) {
            map = lifeCircle(map, false);
        }
        var l0 = formatMap(map, 0);
        var minMax = getMinMaxLevel(map);
        assertEquals( "number of levels after 10 minutes ", 11, minMax.max - minMax.min + 1 );
        assertEquals("number of bugs after 10 minutes", 99, countBugs(map));
    }

    static HashMap<Point,Character> loadMap(String file) {
        final var input = IOUtil.input(file);
        var map = new HashMap<Point,Character>();
        for (int x = 0; x < SIZE; x++ ) {
            for (int y = 0; y < SIZE; y++ ) {
                map.put( new Point(x,y,0), input.get(y).charAt(x) );
            }
        }
        return map;
    }

    @Test
    public void solution() {
        assertEquals( 1, new Point(0,0,0).getBiodiversityRating() );
        assertEquals( 2, new Point(1,0,0).getBiodiversityRating() );
        assertEquals( 4, new Point(2,0,0).getBiodiversityRating() );
        assertEquals( 32768, new Point(0,3, 0).getBiodiversityRating() );

        var map = loadMap("day24");
        if (true) {
            final var history = new HashSet<Long>();
            history.add(getBiodiversityRating(map));
            while (true) {
                map = lifeCircle(map, true);
                final var hash = getBiodiversityRating(map);
                final var isAdded = history.add(hash);
                if ( !isAdded ) break;
            }
            final var rating = getBiodiversityRating(map);
            assertEquals( "answer 1", 18407158, rating );
        }

        map = loadMap("day24");
        for ( int t = 0; t < 200; t++ ) {
            map = lifeCircle(map, false);
        }
        assertEquals( "answer 2", -2, countBugs(map) ); // 99 not right
    }
}