using NUnit.Framework;
using NUnit.Framework.Legacy;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace AdventOfCode2018.Day06
{
    public record Point(int X, int Y)
    {
        public static Distance GetDistance(Point a, Point b)
            => new Distance(Math.Abs(a.X - b.X), Math.Abs(a.Y - b.Y));
        public override string ToString() => $"({X},{Y})";
    }

    public record PointId(int X, int Y, int Id) : Point(X, Y)
    {
        public override string ToString() => string.Format("(({0},{1}),{2})", X, Y, Id);
    }

    public record Distance(int X, int Y)
    {
        public int GetManhattan() => X + Y;
    }

    public class Day6
    {
        // [Ignore("2 seconds")]
        [TestCase("Day06/sample.txt", 17, 32, 16)]
        [TestCase("Day06/input.txt", 3223, 10000, 40495)]
        public void Solution(string file, int expectedLargestArea, int maxDistance, int maxDistanceArea)
        {
            var lines = File.ReadAllLines(Path.Combine(App.Directory, file));
            var pId = 0;
            var points = lines.Select(l =>
            {
                var a = l.Split(',');
                return new PointId(Convert.ToInt32(a[0]),
                    Convert.ToInt32(a[1]), pId++);
            }).ToArray();
            var maxX = points.Max(p => p.X);
            var maxY = points.Max(p => p.Y);
            const int NoId = -1;
            var map = new Dictionary<Point, int>();
            for (int x = 0; x <= maxX; x++)
            {
                for (int y = 0; y <= maxY; y++)
                {
                    var p = new Point(x, y);
                    var pd = points.Select(a => (a, Point.GetDistance(a, p).GetManhattan())).ToArray();
                    var min = pd.Select(a => a.Item2).Min();
                    var minPd = pd.Where(a => a.Item2 == min).ToArray();
                    map[p] = minPd.Length == 1 ? minPd.Single().Item1.Id : NoId;
                }
            }
            var idArea = points.ToDictionary(a => a.Id, a => map.Count(b => b.Value == a.Id));
            for (int x = 0; x <= maxX; x++)
            {
                for (int y = 0; y <= maxY; y++)
                {
                    if (x == 0 || x == maxX || y == 0 || y == maxY)
                    {
                        var p = new Point(x, y);
                        var id = map[p];
                        idArea.Remove(id);
                    }
                }
            }
            var max = idArea.Max(a => a.Value);
            ClassicAssert.AreEqual(expectedLargestArea, max, "answer 1");

            int area = 0;
            for (int x = 0; x <= maxX; x++)
            {
                for (int y = 0; y <= maxY; y++)
                {
                    var p = new Point(x, y);
                    var isOut = points.Select(a => Point.GetDistance(p, a).GetManhattan()).Sum() >= maxDistance;
                    area += isOut ? 0 : 1;
                }
            }
            ClassicAssert.AreEqual(maxDistanceArea, area, "answer 2");
        }
    }
}
