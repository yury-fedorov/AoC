using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using NUnit.Framework;

namespace AdventOfCode2018.Day23 {
    public record Point3D(int X, int Y, int Z) { }

    public record Nanobot(Point3D C, int R) { } // center, radius

    public record Range( int Min, int Max ) { }

    public record Cuboid( Range X, Range Y, Range Z ) { } // right (rectangular) cuboid

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

        public static int Distance(Point3D a, Point3D b)
            => Math.Abs(a.X - b.X) + Math.Abs(a.Y - b.Y) + Math.Abs(a.Z - b.Z);

        public static IEnumerable<Nanobot> InRange(IEnumerable<Nanobot> bots, Point3D p)
            => bots.Where(b => Distance(b.C, p) <= b.R);

        public static int CountPoint(IEnumerable<Nanobot> bots, Point3D p)
            => InRange(bots, p).Count();

        public static Range ? Overlapping( Range a, Range b )
        {
            var r = new Range( Math.Max( a.Min, b.Min ), Math.Min( a.Max, b.Max ) );
            return r.Min <= r.Max ? r : null;
        }

        static Range? OverlappingAxis((int C, int R) a, (int C, int R) b)
        {
            var ar = new Range(a.C - a.R, a.C + a.R);
            var br = new Range(b.C - b.R, b.C + b.R);
            return Overlapping(ar, br);
        }

        static Range? OverlappingAxis( Range ar, (int C, int R) b)
        {
            var br = new Range(b.C - b.R, b.C + b.R);
            return Overlapping(ar, br);
        }

        public static Cuboid ? Overlapping( Nanobot a, Nanobot b )
        {
            var x = OverlappingAxis((a.C.X, a.R), (b.C.X, b.R));
            var y = OverlappingAxis((a.C.Y, a.R), (b.C.Y, b.R));
            var z = OverlappingAxis((a.C.Z, a.R), (b.C.Z, b.R));
            if (x == null || y == null || z == null) return null;
            return new Cuboid( x, y, z );
        }

        public static Cuboid? Overlapping(Cuboid c, Nanobot b)
        {
            var x = OverlappingAxis(c.X, (b.C.X, b.R));
            var y = OverlappingAxis(c.Y, (b.C.Y, b.R));
            var z = OverlappingAxis(c.Z, (b.C.Z, b.R));
            if (x == null || y == null || z == null) return null;
            return new Cuboid(x, y, z);
        }

        public static bool AreTouching( Nanobot a, Nanobot b )
        {
            return AreTouching((a.C.X, a.R), (b.C.X, b.R))
                && AreTouching((a.C.Y, a.R), (b.C.Y, b.R))
                && AreTouching((a.C.Z, a.R), (b.C.Z, b.R));
            
            static bool AreTouching( (int C, int R) a, (int C, int R) b )
            {
                var ar = new Range( a.C - a.R, a.C + a.R );
                var br = new Range( b.C - b.R, b.C + b.R );
                return Overlapping(ar, br) != null;
            }
        }

        public int GetRealShortestDistance( Cuboid area, HashSet<Nanobot> s0 )
        {
            var minD = Distance(P0, new Point3D(area.X.Min, area.Y.Min, area.Z.Min));
            var maxD = Distance(P0, new Point3D(area.X.Max, area.Y.Max, area.Z.Max));

            var shortest = maxD;
            var maxCount = 0;

            for (int x = area.X.Min; x <= area.X.Max; x++)
            {
                for (int y = area.Y.Min; y <= area.Y.Max; y++)
                {
                    for (int z = area.Z.Min; z <= area.Z.Max; z++)
                    {
                        var pi = new Point3D(x, y, z);
                        var ci = CountPoint(s0, pi);
                        var di = Distance(P0, pi);
                        if (ci == s0.Count) return di;
                        if ( ci > maxCount )
                        {
                            maxCount = ci;
                            shortest = di;
                        } else if ( ci == maxCount )
                        {
                            shortest = Math.Min(shortest, di);
                        }
                    }
                }
            }
            return shortest;
        }

        const string FileName = "Day23/input.txt";

        [TestCase("Day23/sample1.txt", 7)]
        [TestCase(FileName, 420)]
        public void Task1(string file, int answer1)
        {
            var lines = File.ReadAllLines(Path.Combine(App.Directory, file));
            var list = lines.Select(Create).ToArray();
            var maxRadius = list.Max(n => n.R);
            var biggest = list.Single(n => n.R == maxRadius);
            var count = list.Where(b => Distance(b.C, biggest.C) <= maxRadius).Count();
            Assert.AreEqual(answer1, count, "answer 1");
        }

        readonly Point3D P0 = new Point3D(0, 0, 0);

        [TestCase("Day23/sample2.txt", 36)]
        [TestCase( FileName, -1)]
        public void Task2(string file, int answer2)
        {
            var lines = File.ReadAllLines(Path.Combine(App.Directory, file));
            var list = lines.Select(Create).ToArray();

            var pc = list.ToDictionary(b => b.C, b => CountPoint(list, b.C));
            var maxNumber = pc.Values.Max();
            
            var p = pc.Where(a => a.Value == maxNumber)
                .OrderBy( a => Distance( P0, a.Key ) ).First();

            var d0 = Distance(P0, p.Key);

            var s0 = new HashSet<Nanobot>( InRange(list, p.Key) );
            var intersection = Overlapping(s0.First(), s0.Last());
            s0.ToList().ForEach(b => intersection = Overlapping(intersection, b));
            var toAdd = new HashSet<Nanobot>(list);
            toAdd.RemoveWhere(e => s0.Contains(e));
            foreach ( var b in toAdd )
            {
                var isToAdd = s0.All(ba => AreTouching(ba, b) );
                if (isToAdd)
                {
                    s0.Add(b);
                    intersection = Overlapping(intersection, b);
                }
            }

            Assert.True( maxNumber < s0.Count, "we optimized more" );

            // var MAX = 125289441; // too high

            var shortest = GetRealShortestDistance(intersection, s0);

            Assert.AreEqual(answer2, shortest, "answer 2");
        }
    }
}
