namespace AoC21;

public class Day15Test
{
    record Point(int X, int Y) {}

    record Cost(long TotalCost, List<Point> Path) {}

    static IEnumerable<Point> Adjusent( int [][] map, Point point ) {
        var isXBelow = (point.X + 1) < map.First().Length;
        var isYBelow = (point.Y + 1) < map.Length;
        if ( isXBelow    ) yield return new Point( point.X + 1, point.Y );
        if ( isYBelow    ) yield return new Point( point.X, point.Y + 1);
        /*
        if ( point.X > 0 ) yield return new Point( point.X - 1, point.Y );
        if ( point.Y > 0 ) yield return new Point( point.X, point.Y - 1 );
        */
    }

    static int RiskAt(int[][] map, Point p) => map[p.Y][p.X];

    static long TotalCost( int[][] map, IEnumerable<Point> path ) => path.Skip(1).Select( p => RiskAt(map, p) ).Sum();

    static bool WasNotHere( IEnumerable<Point> path, Point point ) => !path.Contains( point );

    static List<Point> Compose( IEnumerable<Point> path, Point point ) 
        => Enumerable.Concat( path, new [] { point } ).ToList();

    static Cost Best( IDictionary<Point,Cost> costMap, Cost newCost ) {
        var point = newCost.Path.Last();
        if ( costMap.TryGetValue( point,  out var bestCost ) ) {
            if ( bestCost.TotalCost <= newCost.TotalCost ) return bestCost;
            costMap[point] = newCost;
        } else {
            costMap.Add(point, newCost);
        }
        return newCost;
    }

    [TestCase("Day15/input.txt")]
    // [TestCase("Day15/sample.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var map = lines.Select( _ => _.ToCharArray().Select( ch => ch - '0' ).ToArray() ).ToArray();

        var end = new Point( map.First().Length - 1, map.Length - 1 );
        
        var costMap = new Dictionary<Point,Cost>();
        var start = new Point(0,0);
        var startPath = new List<Point> { start };
        costMap.Add( start, new Cost( TotalCost(map, startPath), startPath ) );

        var ongoingPaths = new Queue< List<Point> >();
        ongoingPaths.Enqueue( startPath );

        while ( ongoingPaths.Any() ) {
            startPath = ongoingPaths.Dequeue();
            start = startPath.Last();
            var nextCosts = Adjusent( map, start ).Where( _ => WasNotHere(startPath, _) )
                .Select( _ => Compose( startPath, _ ) )
                .Select( _ => new Cost( TotalCost(map, _), _ ) );

            nextCosts.ToList().ForEach( cost => Best( costMap, cost ) );
            nextCosts.Where( c => c.Path.Last() != end ).ToList().ForEach( c => ongoingPaths.Enqueue(c.Path) );
        }

        costMap[end].TotalCost.Should().Be(0, "answer 1");
    }
}