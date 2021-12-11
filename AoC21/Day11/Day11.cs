namespace AoC21;

public class Day11Test
{
    // static int CountFlashes( int [][] map ) => map.Select( l => l.Count( _ => _ == 0 ) ).Sum();
    record Point( int X, int Y ) {}

    static IEnumerable<Point> Adjusent( int [][] map, Point point ) {
        var xn = map.First().Length;
        var yn = map.Length;
        var isXBelow = (point.X + 1) < xn;
        var isYBelow = (point.Y + 1) < yn;

        if ( point.X > 0 ) yield return new Point( point.X - 1, point.Y );
        if ( point.Y > 0 ) yield return new Point( point.X, point.Y - 1 );
        if ( isXBelow    ) yield return new Point( point.X + 1, point.Y );
        if ( isYBelow    ) yield return new Point( point.X, point.Y + 1);

        if ( point.X > 0 && point.Y > 0 ) yield return new Point( point.X - 1, point.Y - 1 );
        if ( point.X > 0 && isYBelow    ) yield return new Point( point.X - 1, point.Y + 1 );
        if ( isXBelow    && point.Y > 0 ) yield return new Point( point.X + 1, point.Y - 1 );
        if ( isXBelow    && isYBelow    ) yield return new Point( point.X + 1, point.Y + 1 );
    }

    static int Step( int [][] map ) {
        var flashed = new HashSet<Point>(); 
        var toAdjust = new HashSet<Point>();
        var xn = map.First().Length;
        var yn = map.Length;
        for ( int y = 0; y < yn; y++ ) {
            for ( int x = 0; x < xn; x++ ) {
                var level = map[y][x];
                var newLevel = ( level + 1 > 9 ) ? 0 : ( level + 1 );
                map[y][x] = newLevel;
                if ( newLevel == 0 ) {
                    var p = new Point( x, y );
                    if ( flashed.Add( p ) ) {
                        // An octopus can only flash at most once per step
                        Adjusent( map, p ).ToList().ForEach( _ => toAdjust.Add( _ ) );
                    }
                }
            }
        }
        while ( toAdjust.Any() ) {
            var nextWave = new HashSet<Point>();
            foreach ( var p in toAdjust ) {
                if ( flashed.Contains(p) ) continue;
                var level = map[p.Y][p.X];
                var newLevel = ( level + 1 > 9 ) ? 0 : ( level + 1 );
                map[p.Y][p.X] = newLevel;
                if ( newLevel == 0 ) {
                    if ( flashed.Add( p ) ) {
                        // An octopus can only flash at most once per step
                        Adjusent( map, p ).ToList().ForEach( _ => nextWave.Add( _ ) );
                    }
                }
            }
            toAdjust = nextWave;
        }

        return flashed.Count();
    }

    [TestCase("Day11/sample1.txt", 1)]
    //  [TestCase("Day11/input.txt", 100)]
    public async Task Test(string file, int steps) {
        var lines = await App.ReadLines(file);
        var map = lines.Select( _ => _.ToCharArray().Select( ch => ch - '0' ).ToArray() ).ToArray();
        long flashes = 0;
        for ( int i = 0; i < steps; i++ ) {
            flashes += Step(map);
        }

        Console.WriteLine();
        Enumerable.Range( 0, map.Length ).ToList()
            .ForEach( y => Console.WriteLine( String.Concat( Enumerable.Range( 0, map.First().Length)
                .Select( x => map[y][x] ) ) ) );

        flashes.Should().Be(-1, "answer 1"); // 2281 - not right
    }
}