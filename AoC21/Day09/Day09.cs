namespace AoC21;

public class Day09Test
{
    record Point( int X, int Y ) {}

    static IEnumerable<Point> Adjusent( int [][] map, Point point ) {
        if ( point.X > 0 ) yield return new Point( point.X - 1, point.Y );
        if ( point.Y > 0 ) yield return new Point( point.X, point.Y - 1 );
        if ( (point.X + 1) < map.First().Length ) yield return new Point( point.X + 1, point.Y );
        if ( (point.Y + 1) < map.Length ) yield return new Point( point.X, point.Y + 1);
    }

    static int Height( int [][] map, Point point ) => map[point.Y][point.X];

    static bool IsLocalMin( int [][] map, Point point ) {
        var v = Height(map, point);
        if ( v >= 9 ) return false;
        return Adjusent( map, point ).Select( p => Height(map, p ) ).All( a => a > v );
    }

    static int BasinSize( int [][] map, Point point ) {
        var basin = new Dictionary<Point,int>();
        var next = new Stack<Point>();
        next.Push(point);
        while ( next.Any() ) {
            var p = next.Pop();
            if ( basin.ContainsKey(p) ) continue;
            var h0 = Height(map, p);
            basin.Add( p, h0 );
            Adjusent(map, p).Select( a => ( a, Height(map, a) ) ) // point, height
                .Where( ph => ph.Item2 < 9 && ph.Item2 >= h0 ).ToList().ForEach( _ => next.Push( _.Item1 ) );
        }
        return basin.Count;
    }

    [TestCase("Day09/input.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var values = lines.Select( _ => _.ToCharArray().Select( ch => ch - '0' ).ToArray() ).ToArray();
        var y = values.Length;
        var x = values.First().Length;
        
        var mins = Enumerable.Range( 0, x ).SelectMany( x => Enumerable.Range( 0, y ).Select( y => new Point( x, y ) ) )
            .Where( p => IsLocalMin( values, p ) ).ToList();
        mins.Select( p => ( Height( values, p ) + 1 ) ).Sum().Should().Be(594, "answer 1");

        var maxSize = mins.Select( _ => BasinSize( values, _ ) ).OrderByDescending( _ => _ ).Take(3).ToArray();
        ( maxSize[0] * maxSize[1] * maxSize[2] ).Should().Be(858494, "answer 2");
    }
}