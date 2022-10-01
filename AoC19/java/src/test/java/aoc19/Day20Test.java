package aoc19;

import org.junit.Test;

import java.util.*;

import static org.junit.Assert.assertEquals;

public class Day20Test {

    record Point( int x, int y ) {}
    record Point3D(Point point, int level) {} // used in part 2

    static Set<Point> step(Point p) {
        return Set.of( new Point(p.x, p.y+1), new Point(p.x, p.y-1),
                        new Point(p.x-1, p.y), new Point(p.x+1, p.y) );
    }

    record Maze(Set<Point> walkable, Map<String,List<Point>> doors) {}

    static Maze createMaze( String file ) {
        final var input = IOUtil.input(file);
        final var walkable = new HashSet<Point>();
        final var letters = new HashMap<Point,Character>();

        final var yMax = input.size();
        for ( var y = 0; y < yMax; y++ ) {
            final var line = input.get(y);
            final var xMax = line.length();
            for ( var x = 0; x < xMax; x++ ) {
                final var c = line.charAt(x);
                if ( c == '.' ) walkable.add( new Point(x,y) );
                else if ( Character.isUpperCase(c) ) letters.put( new Point(x,y), c );
            }
        }
        return new Maze(walkable, createDoors(letters, walkable));
    }

    static boolean isClose(Point a, Point b) { return step(a).contains(b); }
    static Point getPoint(Point a, Point b, Set<Point> walkable) {
        final var s = new HashSet<>(step(a));
        s.addAll(step(b));
        s.retainAll(walkable);
        return s.iterator().next();
    }

    static String getName(Point ap, char ac, Point bp, char bc) {
        final var isSameX = ap.x == bp.x;
        final var isAb = isSameX ? ap.y < bp.y : ap.x < bp.x;
        return isAb ? ( "" + ac ) + bc : ( "" + bc ) + ac;
    }

    static Map<String,List<Point>> createDoors(Map<Point,Character> letters, Set<Point> walkable) {
        final var doors = new HashMap<String,List<Point>>();
        final var points = new ArrayList<>( letters.keySet() );
        while ( !points.isEmpty() ) {
            final var a = points.remove(0);
            Point b = null;
            for ( var i : points ) {
                if ( isClose(a, i) ) {
                    b = i;
                    points.remove(b);
                    break;
                }
            }
            final var name = getName( a, letters.get(a), b, letters.get(b) );
            final var d = doors.getOrDefault( name, new ArrayList<>(2) );
            d.add( getPoint( a, b, walkable ) );
            doors.put(name, d);
        }
        return doors;
    }

    static int solution(boolean isPart1, Maze maze) {
        // TODO implement the algorithm
        int level = 0; // 0 - the outermost level
        final var aa = maze.doors.get("AA").get(0);
        var next = Map.of( new Point3D(aa, level), 0); // generalized next
        var minDistance = new HashMap<Point3D,Integer>(); // generalized min distance
        final var zz = maze.doors.get("ZZ").get(0);
        // 1 is it possible to walk from AA to ZZ directly without portals?
        // this is the maximum path required to arrive
        // after this options are: look in the range of minimal path all reachable portals
        // try to go measure paths using them
        while (!next.isEmpty()) {
            final var next1 = new HashMap<Point3D, Integer>();
            for ( var n : next.entrySet() ) {
                final var p = n.getKey();
                final var minD = minDistance.getOrDefault( p, Integer.MAX_VALUE ).intValue();
                final var d = n.getValue().intValue();
                if ( d < minD ) {
                    minDistance.put( p, d );
                } else {
                    continue; // no sense to continue
                }
                final var p1 = new HashSet<>( step(p.point) );
                p1.retainAll(maze.walkable);
                // TODO
                final var di = maze.doors.values().stream()
                        .filter(c -> c.size() == 2 && c.contains(p.point) ).iterator();
                if ( di.hasNext() ) {
                    // we found a portal
                    p1.addAll( di.next() );
                    p1.remove(p.point);
                }
                for ( var p1i : p1 ) {
                    final var d1 = d+1;
                    final var d1i = next1.getOrDefault(p1i, Integer.MAX_VALUE);
                    if ( d1 < d1i ){
                        next1.put( new Point3D(p1i,level), d1 );
                    }
                }
            }
            next = next1;
        }
        return minDistance.get( new Point3D( zz, 0 ) ).intValue();
    }

    @Test
    public void solution() {
        final var maze = createMaze("day20");
        assertEquals( "answer 1", 580, solution(true, maze) );
        assertEquals( "answer 2", -2, solution(false, maze) );
    }
}
