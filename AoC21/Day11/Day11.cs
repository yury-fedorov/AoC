namespace AoC21;

public class Day11Test
{
    record Point( int X, int Y ) {}

    static IEnumerable<Point> Adjusent( int [][] map, Point point ) {
        var isXBelow = (point.X + 1) < map.First().Length;
        var isYBelow = (point.Y + 1) < map.Length;

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
        var toAdjust = new List<Point>();
        var xn = map.First().Length;
        var yn = map.Length;
        for ( int y = 0; y < yn; y++ ) {
            for ( int x = 0; x < xn; x++ ) {
                var level = map[y][x] + 1;
                map[y][x] = level > 9 ? 0 : level;
                if ( level > 9 ) {
                    var p = new Point( x, y );
                    if ( flashed.Add( p ) ) {
                        // An octopus can only flash at most once per step
                        Adjusent( map, p ).ToList().ForEach( _ => toAdjust.Add( _ ) );
                    }
                }
            }
        }
        while ( toAdjust.Any() ) {
            var nextWave = new List<Point>();
            foreach ( var p in toAdjust ) {
                if ( flashed.Contains(p) ) continue;
                var level = map[p.Y][p.X] + 1;
                map[p.Y][p.X] = level > 9 ? 0 : level;
                if ( level > 9 ) {
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

    // [TestCase("Day11/sample1.txt", 2)]
    [TestCase("Day11/input.txt", 100)]
    public async Task Test(string file, int steps) {
        var lines = await App.ReadLines(file);
        var map = lines.Select( _ => _.ToCharArray().Select( ch => ch - '0' ).ToArray() ).ToArray();
        var size = map.Length * map.First().Length; // total size of the map
        int flashes = 0;
        for ( int step = 0; true; step++ ) {
            int justFlashed = Step(map);
            if ( step < steps ) flashes += justFlashed;
            if ( justFlashed == size ) {
                ( step + 1 ).Should().Be(329, "answer 2");
                break;
            }
        }

        flashes.Should().Be(1627, "answer 1");
    }
}