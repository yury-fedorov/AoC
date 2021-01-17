using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using NUnit.Framework;

namespace AdventOfCode2018.Tests
{

    public class Point { 
        public int X;  public int Y; 

        public static Distance GetDistance(Point a, Point b)
        {
            return new Distance { X = Math.Abs(a.X - b.X), Y = Math.Abs(a.Y - b.Y) };
        }

        public override string ToString()
        {
            return string.Format("({0},{1})", X,Y);
        }

        public override bool Equals(object obj)
        {
            return ToString().Equals(obj.ToString());
        }

        public override int GetHashCode()
        {
            return ToString().GetHashCode();
        }
    }

    public class PointId : Point { public int Id;

        public override string ToString()
        {
            return string.Format("(({0},{1}),{2})", X, Y, Id);
        }
    }

    public class Distance {
        public int X; public int Y;

        public int GetManhattan() {
            return X + Y;
        }
    }

    public class Day6Test
    {
        [Ignore("2 seconds")]
        [TestCase("Day6Sample.txt", 17,32,16)]
        [TestCase("Day6Input.txt",3223,10000, 40495)]
        public void TestCase1(string file, int expectedLargestArea, int maxDistance, int maxDistanceArea) {
            var lines = File.ReadAllLines(Path.Combine(App.Directory, file));
            var pId = 0;
            var points = lines.Select(l => {
                var a = l.Split(',');
                return new PointId { X = Convert.ToInt32(a[0]), 
                    Y = Convert.ToInt32(a[1]), Id = pId++ }; }).ToArray();
            var maxX = points.Max(p => p.X);
            var maxY = points.Max(p => p.Y);
            const int NoId = -1;

            var map = new Dictionary<Point, int>();

            for (int x = 0; x <= maxX; x++)
            {
                for (int y = 0; y <= maxY; y++)
                {
                    var p = new Point { X = x, Y = y };
                    // if (map[p] == ND) 
                    {
                        var pd = points.Select(a => Tuple.Create(a,Point.GetDistance(a, p).GetManhattan())).ToArray();
                        var min = pd.Select(a => a.Item2).Min();
                        var minPd = pd.Where(a => a.Item2 == min).ToArray();
                        map[p] = minPd.Length == 1 ? minPd.Single().Item1.Id : NoId;
                    }
                }
            }
            var idArea = points.ToDictionary(a => a.Id, a=>  map.Count(b => b.Value == a.Id));
            for (int x = 0; x <= maxX; x++)
            {
                for (int y = 0; y <= maxY; y++)
                {
                    if (x == 0 || x == maxX || y == 0 || y == maxY ) {
                        var p = new Point { X = x, Y = y };
                        var id = map[p];
                        idArea.Remove(id);
                    }
                }
            }
            var max = idArea.Max(a => a.Value);
            Assert.AreEqual(expectedLargestArea, max); // answer 1

            int area = 0;
            for (int x = 0; x <= maxX; x++)
            {
                for (int y = 0; y <= maxY; y++)
                {
                    var p = new Point { X = x, Y = y };
                    var isOut = points.Select(a => Point.GetDistance(p, a).GetManhattan()).Sum() >= maxDistance;
                    area += isOut ? 0 : 1;
                }
            }
            Assert.AreEqual(maxDistanceArea, area, "wrong area");
        }
    }
}
