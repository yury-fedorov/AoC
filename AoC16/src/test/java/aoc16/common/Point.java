package aoc16.common;

public record Point ( int x, int y) {

    public static Point with( int x, int y ) { return new Point(x,y); }

    public static int distance(Point a, Point b ) {
        return Math.abs( a.x - b.x ) + Math.abs( a.y - b.y );
    }

    public static Point add( Point a, Point b ) { return Point.with( a.x + b.x, a.y + b.y ); }
}
