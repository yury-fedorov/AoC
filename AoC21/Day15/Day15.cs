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
        if ( point.X > 0 ) yield return new Point( point.X - 1, point.Y );
        if ( point.Y > 0 ) yield return new Point( point.X, point.Y - 1 );
    }

    static int RiskAt(int[][] map, Point p) => map[p.Y][p.X];

    static long TotalCost( int[][] map, IEnumerable<Point> path ) => path.Skip(1).Select( p => RiskAt(map, p) ).Sum();

    static bool WasNotHere( IEnumerable<Point> path, Point point ) => !path.Contains( point );

    static List<Point> Compose( IEnumerable<Point> path, Point point ) 
        => Enumerable.Concat( path, new [] { point } ).ToList();

    static bool Best( IDictionary<Point,Cost> costMap, Cost newCost ) {
        var point = newCost.Path.Last();
        if ( costMap.TryGetValue( point,  out var bestCost ) ) {
            if ( bestCost.TotalCost <= newCost.TotalCost ) return false; // old cost is better
            costMap[point] = newCost;
        } else {
            costMap.Add(point, newCost);
        }
        return true;
    }

    static long TotalCost(int [][] map)
    {
        var end = new Point(map.First().Length - 1, map.Length - 1);

        var costMap = new Dictionary<Point, Cost>();
        var start = new Point(0, 0);
        var startPath = new List<Point> { start };
        costMap.Add(start, new Cost(TotalCost(map, startPath), startPath));

        var ongoingPaths = new Queue<List<Point>>();
        ongoingPaths.Enqueue(startPath);

        while (ongoingPaths.Any())
        {
            startPath = ongoingPaths.Dequeue();
            start = startPath.Last();
            var nextCosts = Adjusent(map, start).Where(_ => WasNotHere(startPath, _))
                .Select(_ => Compose(startPath, _))
                .Select(_ => new Cost(TotalCost(map, _), _));

            nextCosts.Where(cost => Best(costMap, cost)).Where(c => c.Path.Last() != end).ToList().ForEach(c => ongoingPaths.Enqueue(c.Path));
        }

        return costMap[end].TotalCost;
    }

    static int Map2( int[][] map, Point p )
    {
        var sizeX = map.First().Length;
        var sizeY = map.Length;
        int indexX = p.X / sizeX;
        int indexY = p.Y / sizeY;
        var shift = indexX + indexY;
        var value = map[p.Y % sizeY][p.X % sizeX];
        var shiftedValue = value + shift;
        return shiftedValue <= 9 ? shiftedValue : shiftedValue - 9; // 10 -> 1, 11 -> 2
    }

    [TestCase("Day15/input.txt")]
    // [TestCase("Day15/sample.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var map = lines.Select( _ => _.ToCharArray().Select( ch => ch - '0' ).ToArray() ).ToArray();
        // TotalCost(map).Should().Be(537, "answer 1"); // 41 seconds
        
        var sizeX = map.First().Length;
        var sizeY = map.Length;
        var sizeX1 = 5 * sizeX;
        var sizeY1 = 5 * sizeY;
        var map5x = Enumerable.Range(0, sizeY1).Select(y => Enumerable.Range(0, sizeX1).Select(x => Map2(map, new Point(x, y))).ToArray()).ToArray();
        TotalCost(map5x).Should().Be(-1, "answer 2"); //
    }
}
