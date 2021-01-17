using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using NUnit.Framework;

namespace AdventOfCode2018
{
    public record Point3D(int X, int Y, int Z) { }

    public record Nanobot(Point3D P, int R) { } // center, radius

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

        public static int Distance(Point3D a, Point3D b) => Math.Abs(a.X - b.X) + Math.Abs(a.Y - b.Y) + Math.Abs(a.Z - b.Z);

        public static int CountPoint(IEnumerable<Nanobot> bots, Point3D p) => bots.Count(b => Distance(b.P, p) <= b.R);

        [TestCase("Day23/input.txt", 420, 1)]
        public void Test(string file, int eCount, int eDistance)
        {
            var lines = File.ReadAllLines(Path.Combine(App.Directory, file));
            var list = lines.Select(Create).ToArray();
            var maxRadius = list.Max(n => n.R);
            var biggest = list.Single(n => n.R == maxRadius);
            var count = list.Where(b => Distance(b.P, biggest.P) <= maxRadius).Count();
            Assert.AreEqual(eCount, count, "answer 1");

            Assert.Fail( "No correct answer for task 2" );

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
                }

                var py = new Point3D(p1.X, p1.Y - dy, p1.Z);
                var cy = CountPoint(list, py);
                if (cy >= maxNumber)
                {
                    maxNumber = cy;
                    p1 = py;
                }

                var pz = new Point3D(p1.X, p1.Y, p1.Z - dz);                
                var cz = CountPoint(list, pz);
                if (cz >= maxNumber)
                {
                    maxNumber = cz;
                    p1 = pz;
                }
                if ( Distance(p0, p1) >= Distance(p0, cp1) ) break;
            }
            
            var MAX = 125289441;
            int shortestDistance = MAX;
            /*
            for ( int x = 1; x < shortestDistance; x++ ) {
                for ( int y = shortestDistance - x; y > 0; y-- ) {
                    for ( int z = shortestDistance - x - y; z > 0; z-- ) {
                        var pi = new Point3D( x, y, z );
                        var ci = CountPoint(list, pi);
                        var di = Distance(p0,pi);
                        if ( maxNumber <= ci && shortestDistance > di ) {
                            shortestDistance = di;
                            maxNumber = ci;
                        }
                    }
                }
            }
            */

            // 125302823 -- too high
            // 125289441 -- too high
            Assert.AreEqual(-1, shortestDistance, "answer 2");
        }
    }
}
