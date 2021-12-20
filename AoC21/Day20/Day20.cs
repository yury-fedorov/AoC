namespace AoC21;

public class Day20Test
{
    record Point ( int X, int Y ) {}

    static IEnumerable<Point> Adjusent( Point p ) => Enumerable.Range(-1,3)
        .SelectMany( dy => Enumerable.Range(-1,3).Select( dx => new Point(p.X + dx,p.Y + dy) ) );

    static bool IsOn( HashSet<Point> binaryMap, Point point, Range r, bool isOn ) {
        if ( r.Min.X <= point.X && point.X <= r.Max.X && r.Min.Y <= point.Y && point.Y <= r.Max.Y )
            return binaryMap.Contains(point);
        return isOn;
    }
    
    static int Index( HashSet<Point>  binaryMap, Point point, Range r, bool isOn ) {
        var intAsStr = string.Concat( Adjusent(point).Select( p => IsOn( binaryMap, p, r, isOn ) ? '1' : '0' ) );
        return Convert.ToInt32( intAsStr, 2 );
    }

    static bool IsNewOn( string algorithm, int index ) => algorithm[index] == '#';

    record Range( Point Min, Point Max ) {}

    static Range MinMax1( HashSet<Point> map, int d = 1 ) {
        var minx = map.Select( p => p.X ).Min();
        var maxx = map.Select( p => p.X ).Max();
        var miny = map.Select( p => p.Y ).Min();
        var maxy = map.Select( p => p.Y ).Max();
        return new Range( new Point( minx - d, miny - d ), new Point( maxx + d, maxy + d ) );
    }

    static Range Extend( Range r, int d ) 
        => new Range( new Point( r.Min.X - d, r.Min.Y - d ), new Point( r.Max.X + d, r.Max.Y + d ) );

    static int Count( int min, int max ) => max - min + 1;

    static IEnumerable<Point> Points( Range r ) => Enumerable.Range( r.Min.Y, Count(r.Min.Y, r.Max.Y) )
        .SelectMany( y => Enumerable.Range( r.Min.X, Count(r.Min.X, r.Max.X) ).Select( x => new Point(x,y) ) );

    static int Run( HashSet<Point> input, string algorithm, int steps ) {
        bool isOn = false;
        var r0 = MinMax1(input, 0);
        var r = MinMax1( input );
        var isNewOn = (Point p) => IsNewOn( algorithm, Index( input, p, r0, isOn ) );
        var eternalPoint = new Point(int.MinValue, int.MinValue);
        for ( int step = 0; step < steps; step++ ) {
            input = Points(r).Where( isNewOn ).ToHashSet();
            r0 = Extend(r0, 1);
            r = Extend(r,1);
            isOn = isNewOn( eternalPoint );
        }
        return input.Count();
    }

    [TestCase("Day20/input.txt")]
    //  [TestCase("Day20/sample.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var algorithm = lines.First();
        var map = lines.Skip(2).ToArray();
        var ys = map.Length;
        var xs = map.First().Length();
        var input = Enumerable.Range( 0, ys )
            .SelectMany( y => Enumerable.Range( 0, xs ).Select( x => new Point(x,y) ) )
            .Where( p => map[p.Y][p.X] == '#' ).ToHashSet();
        if (App.IsFast) return; // 3 seconds
        Run(input, algorithm, 2).Should().Be(5057, "answer 1");
        Run(input, algorithm, 50).Should().Be(18502, "answer 2"); 
    }
}