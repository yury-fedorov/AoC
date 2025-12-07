namespace AoC21;

public class Day05Test
{
    record Point(int X, int Y) { }
    record Line(Point A, Point B) { }

    static Line ParseLine(string line)
    {
        // 405,945 -> 780,945
        var matches = Regex.Matches(line, @"(\d+),(\d+) -> (\d+),(\d+)");
        foreach (Match match in matches)
        {
            var x1 = Convert.ToInt32(match.Groups[1].Value);
            var y1 = Convert.ToInt32(match.Groups[2].Value);
            var x2 = Convert.ToInt32(match.Groups[3].Value);
            var y2 = Convert.ToInt32(match.Groups[4].Value);
            return new Line(new Point(x1, y1), new Point(x2, y2));
        }
        throw new Exception($"bad line: {line}");
    }

    static bool IsHorizontal(Line line) => line.A.Y == line.B.Y;
    static bool IsVertical(Line line) => line.A.X == line.B.X;

    static IEnumerable<Point> AllPoints(Line line, bool isDiagonal)
    {
        if (!isDiagonal && !IsHorizontal(line) && !IsVertical(line)) return Enumerable.Empty<Point>();
        var dx = Math.Sign(line.B.X - line.A.X);
        var dy = Math.Sign(line.B.Y - line.A.Y);
        var x = line.A.X;
        var y = line.A.Y;
        var result = new List<Point>();
        while (true)
        {
            result.Add(new Point(x, y));
            if ((dx != 0 && x == line.B.X) || (dy != 0 && y == line.B.Y)) break;
            x += dx;
            y += dy;
        }
        return result;
    }

    static void TestLine(Line line, int expectedSize, Point expectedPoint)
    {
        var result = AllPoints(line, true).ToList();
        result.Count().Should().Be(expectedSize);
        result.Contains(expectedPoint).Should().BeTrue();
    }

    [Test]
    public void DiagonalTest()
    {
        TestLine(new Line(new Point(1, 1), new Point(3, 3)), 3, new Point(2, 2));
        TestLine(new Line(new Point(9, 7), new Point(7, 9)), 3, new Point(8, 8));
    }

    static int Answer(List<Line> lines, bool isDiagonal)
        => lines.SelectMany(l => AllPoints(l, isDiagonal)).GroupBy(_ => _)
            .ToDictionary(_ => _.Key, _ => _.Count()).Count(p => p.Value > 1);

    [TestCase("Day05/input.txt")]
    public async Task Test(string file)
    {
        var textLines = await App.ReadLines(file);
        var lines = textLines.Select(ParseLine).ToList();
        Answer(lines, false).Should().Be(7438, "answer 1");
        Answer(lines, true).Should().Be(21406, "answer 2");
    }
}