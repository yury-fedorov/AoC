using System;
using System.Collections.Generic;
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

        public int CountPoint(IEnumerable<Nanobot> bots, Point3D point) =>
            bots.Where(b => Distance(b.P, point) <= b.R).Count();

        // 125302823 -- too high
        [TestCase("Day23.txt", 420, 1)]
        public void Test(string file, int eCount, int eDistance)
        {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file));
            var list = lines.Select(Create).ToArray();
            var maxRadius = list.Max(n => n.R);
            var biggest = list.Single(n => n.R == maxRadius);
            var count = list.Where(b => Distance(b.P, biggest.P) <= maxRadius).Count();
            Assert.AreEqual(eCount, count);

            var pc = list.ToDictionary(b => b.P, b => CountPoint(list, b.P)); // 870
            var maxNumber = pc.Values.Max();
            var p = pc.Single(a => a.Value == maxNumber);
            var p0 = new Point3D(0, 0, 0);
            var d0 = Distance(p0, p.Key);
            var p1 = p.Key;

            var dx = Math.Sign(p1.X);
            var dy = Math.Sign(p1.Y);
            var dz = Math.Sign(p1.Z);

            while ( true )
            {
                var cp1 = p1; // previous point

                var px = new Point3D(p1.X - dx, p1.Y, p1.Z);
                var cx = CountPoint(list, px);
                if (cx >= maxNumber)
                {
                    maxNumber = cx;
                    p1 = px;
                    Assert.True(Distance(p0, p1) < Distance(p0, cp1));
                    continue;
                }

                var py = new Point3D(p1.X, p1.Y - dy, p1.Z);
                var cy = CountPoint(list, py);
                if (cy >= maxNumber)
                {
                    maxNumber = cy;
                    p1 = py;
                    continue;
                }

                var pz = new Point3D(p1.X, p1.Y, p1.Z - dz);                
                var cz = CountPoint(list, pz);
                if (cz >= maxNumber)
                {
                    maxNumber = cz;
                    p1 = pz;
                    continue;
                }
                break;
            }

            Assert.AreEqual(-1, Distance(p0,p1));
        }
    }
}
