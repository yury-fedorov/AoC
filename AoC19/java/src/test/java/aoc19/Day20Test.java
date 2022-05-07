package aoc19;

import org.junit.Test;

import java.util.*;

import static org.junit.Assert.assertEquals;

public class Day20Test {

    record Point( int x, int y ) {}

    static Set<Point> step(Point p) {
        return Set.of( new Point(p.x, p.y+1), new Point(p.x, p.y-1),
                        new Point(p.x-1, p.y), new Point(p.x+1, p.y) );
    }

    record Maze(Set<Point> walkable, Map<String,List<Point>> doors) {}
    static Point otherPortal( List<Point> portalDoors, Point entrance ) {
        final var a = portalDoors.get(0);
        final var b = portalDoors.get(1);
        return a.equals(entrance) ? b : a;
    }

    static Maze createMaze( String file ) {
        final var input = IOUtil.input(file);
        final var walkable = new HashSet<Point>();
        final var letters = new HashMap<Point,Character>();
        final var doors = new HashMap<String,List<Point>>();
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
        // TODO based on letters and walkable - construct doors
        return new Maze(walkable, doors);
    }

    @Test
    public void solution() {
        final var maze = createMaze("day20");
        // TODO implement the algorithm
        // 1 is it possible to walk from AA to ZZ directly without portals?
        // this is the maximum path required to arrive
        // after this options are: look in the range of minimal path all reachable portals
        // try to go measure paths using them
        assertEquals( "answer 1", -1, 0);
        assertEquals( "answer 2", -2, 0 );
    }
}
