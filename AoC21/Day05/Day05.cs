namespace AoC21;

public class Day05Test
{
    record Point( int X, int Y ) {}
    record Line( Point A, Point B ) {}

    // 405,945 -> 780,945
    static Line ParseLine( string line ) {
        var matches = Regex.Matches(line, @"(\d+),(\d+) -> (\d+),(\d+)");
        foreach (Match match in matches) {
            var x1 = Convert.ToInt32(match.Groups[1].Value);
            var y1 = Convert.ToInt32(match.Groups[2].Value);
            var x2 = Convert.ToInt32(match.Groups[3].Value);
            var y2 = Convert.ToInt32(match.Groups[4].Value);
            return new Line( new Point(x1,y1), new Point(x2,y2) );
        }
        throw new Exception($"bad line: {line}" );
    }

    static bool IsHorizontal( Line line ) => line.A.Y == line.B.Y;
    static bool IsVertical( Line line ) => line.A.X == line.B.X;

    // XXX: only for vertial and horizontal 
    static IEnumerable<Point> AllPoints(Line line, bool isDiagonal ) {
        if ( IsHorizontal(line) ) {
            var x0 = Math.Min( line.A.X, line.B.X );
            var x1 = Math.Max( line.A.X, line.B.X );
            return Enumerable.Range( x0, x1 - x0 + 1 ).Select( x => new Point(x, line.A.Y) );
        }
        if ( IsVertical(line) ) {
            var y0 = Math.Min( line.A.Y, line.B.Y );
            var y1 = Math.Max( line.A.Y, line.B.Y );
            return Enumerable.Range( y0, y1 - y0 + 1 ).Select( y => new Point(line.A.X, y) );
        }
        if ( !isDiagonal ) return Enumerable.Empty<Point>();
        var dx = Math.Sign( line.B.X - line.A.X );
        var dy = Math.Sign( line.B.Y - line.A.Y );
        var x = line.A.X;
        var y = line.A.Y;
        var result = new List<Point>();
        while (true) {
            result.Add( new Point(x,y) );
            if ( x == line.B.X ) break;
            x += dx;
            y += dy;
        };
        return result;
    }

    [Test]
    public void DiagonalTest() {
        var line = new Line( new Point(1,1), new Point(3,3) );
        var result = AllPoints( line, true ).ToList();
        result.Count().Should().Be(3);
        result.Contains(new Point(2,2)).Should().BeTrue();

        line = new Line( new Point(9,7), new Point(7,9) );
        result = AllPoints( line, true ).ToList();
        result.Count().Should().Be(3);
        result.Contains(new Point(8,8)).Should().BeTrue();
    }

    [TestCase("Day05/input.txt")]
    public async Task Test(string file) {
        var textLines = await App.ReadLines(file);
        List<Line> lines = textLines.Select(ParseLine).ToList();
        
        var a1 = lines.SelectMany( l => AllPoints(l, false) )
            .GroupBy( _ => _ )
            .ToDictionary( _ => _.Key, _ => _.Count() );
        
        a1.Count( p => p.Value > 1 ).Should().Be(7438, "answer 1");

        var a2 = lines.SelectMany( l => AllPoints(l, true) )
            .GroupBy( _ => _ )
            .ToDictionary( _ => _.Key, _ => _.Count() );

        a2.Count( p => p.Value > 1 ).Should().Be(21406, "answer 2");
    }
}