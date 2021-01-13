package aoc16.common;

import java.util.Objects;

public class Point {
    public final int x;
    public final int y;
    public Point(int x, int y) { this.x = x; this.y = y; }
    public static Point with( int x, int y ) { return new Point(x,y); }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof Point)) return false;
        Point point = (Point) o;
        return x == point.x && y == point.y;
    }

    @Override
    public int hashCode() { return Objects.hash(x, y); }

    public static int distance(Point a, Point b ) {
        return Math.abs( a.x - b.x ) + Math.abs( a.y - b.y );
    }
}
