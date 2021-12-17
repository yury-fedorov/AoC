using System.Collections.Concurrent;

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

    static long TotalCost( int[][] map, Cost cost0, Point last ) => cost0.TotalCost + RiskAt(map, last);

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

    static IEnumerable<Cost> Next( int [][] map, Cost startCost) {
        var startPath = startCost.Path;
        var start = startPath.Last();
        return Adjusent(map, start).Where(_ => WasNotHere(startPath, _))
            .Select(_ => new Cost(TotalCost(map, startCost, _), Compose(startPath, _) )).ToList();
    }

    static long Priority(Cost cost) {
        var last = cost.Path.Last();
        var n = cost.Path.Count();
        return -( last.X + last.Y ) // more we ahead on the field, more we are close to at least one solution
        + n // the longer the queue, the less it is interesting
        + ( cost.TotalCost / n ); // less total normalized cost - better the solution
    }

    static long TotalCost(int [][] map)
    {
        var end = new Point(map.First().Length - 1, map.Length - 1);

        var costMap = new ConcurrentDictionary<Point, Cost>();
        var start = new Point(0, 0);
        var startPath = new List<Point> { start };
        var startCost = new Cost(TotalCost(map, startPath), startPath);
        costMap.TryAdd(start, startCost);

        var ongoingPaths = new PriorityQueue<Cost,long>();
        ongoingPaths.Enqueue(startCost, Priority(startCost) );
        var tasks = new List< Task< IEnumerable<Cost> > >();

        while ( true )
        {
            if ( ongoingPaths.Count > 0 ) {
                startCost = ongoingPaths.Dequeue();
                var cost = startCost;
                var l = () => Next( map, cost );
                tasks.Add( Task.Run( l ) );
            }  else {
                if ( !tasks.Any() ) break;
                Task.WaitAll( tasks.ToArray() );
                tasks.SelectMany( t => t.Result )
                .Where(cost => Best(costMap, cost)).Where(c => c.Path.Last() != end)
                .ToList().ForEach( r => ongoingPaths.Enqueue(r, Priority(r)) );
                tasks.Clear();
            }
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
        var value = RiskAt( map, new Point( p.X % sizeX, p.Y % sizeY ) );
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
