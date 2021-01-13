package aoc16.common;

import org.javatuples.Pair;

import java.util.Objects;

public class Point {
    final Pair<Integer,Integer> point;
    public Point(int x, int y) { point = Pair.with(x, y); }
    public static Point with( int x, int y ) { return new Point(x,y); }
    public int x() { return point.getValue0(); }
    public int y() { return point.getValue1(); }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof Point)) return false;
        Point point1 = (Point) o;
        return Objects.equals(point, point1.point);
    }

    @Override
    public int hashCode() { return Objects.hash(point); }

    public int distance( Point o ) { return distance( this, o ); }

    public static int distance(Point a, Point b ) {
        return Math.abs( a.x() - b.y() ) + Math.abs( a.x() - b.y() );
    }
}
