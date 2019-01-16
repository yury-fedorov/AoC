using System;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using NUnit.Framework;

namespace AdventOfCode2018
{
    public class Point3D : Tuple<int,int,int> {
        public Point3D(int x, int y, int z) : base(x, y, z) { }
        public int X => Item1;
        public int Y => Item2;
        public int Z => Item3;
    }

    public class Nanobot : Tuple<Point3D,int>
    {
        public Nanobot(Point3D center, int radius) : base(center,radius) {}
        public Point3D P => Item1; // point
        public int R => Item2;
    }

    public class Day23
    {
        public static Nanobot Create(string line)
        {
            var matches = Regex.Matches(line, "pos=<(.?[0-9]+),(.?[0-9]+),(.?[0-9]+)>, r=([0-9]+)");
            foreach (Match match in matches)
            {
                var center = new Point3D(Convert.ToInt32(match.Groups[1].Value), Convert.ToInt32(match.Groups[2].Value), Convert.ToInt32(match.Groups[3].Value));
                return new Nanobot ( center, Convert.ToInt32(match.Groups[4].Value) );
            }
            throw new Exception("unexpected format");
        }

        public int Distance(Point3D a, Point3D b) => Math.Abs(a.X - b.X) + Math.Abs(a.Y - b.Y) + Math.Abs(a.Z - b.Z);

        [TestCase("Day23.txt")]
        public void Test(string file)
        {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file));
            var list = lines.Select(Create).ToArray();
            var maxRadius = list.Max(n => n.R);
            var biggest = list.Single(n => n.R == maxRadius);
            var count = list.Where(b => Distance(b.P, biggest.P) <= maxRadius).Count();
            Assert.AreEqual(-1, count);
        }
    }
}
