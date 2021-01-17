using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using NUnit.Framework;

namespace AdventOfCode2018.Day25
{
    public class Point4D : Tuple<int,int,int,int>
    {
        public Point4D(int d1, int d2, int d3, int d4) : base(d1,d2,d3,d4) {}
        public int A => Item1;
        public int B => Item2;
        public int C => Item3;
        public int D => Item4;

        public readonly HashSet<Point4D> Neighbours = new HashSet<Point4D>();
    }

    public class Castellation 
    {
        public const int MaxDistance = 3;

        public static int Distance(Point4D a, Point4D b)
        {
            return Math.Abs(a.A - b.A)
                 + Math.Abs(a.B - b.B)
                 + Math.Abs(a.C - b.C)
                 + Math.Abs(a.D - b.D);
        }

        public static HashSet<Point4D> DefineCastellation(Point4D firstPoint)
        {
            var set = new HashSet<Point4D>();
            set.Add(firstPoint);
            var toProcess = new HashSet<Point4D>(firstPoint.Neighbours);
            while(toProcess.Any())
            {
                var point = toProcess.First();
                if (!set.Contains(point))
                {
                    // this point to be within castellation but was not processed
                    set.Add(point);
                    foreach(var n in point.Neighbours)
                    {
                        if (!set.Contains(n))
                        {
                            // this point to be processed as soon as it is not yet the part of the castellation
                            toProcess.Add(n);
                        }
                    }
                }
                toProcess.Remove(point);
            }
            return set;
        }
    }

    public class Day25
    {
        public static IEnumerable<Point4D> Read(string file)
        {
            var lines = File.ReadAllLines(Path.Combine(App.Directory, file));
            return lines.Select(l => {
                var a = l.Split(",").Select(s => Convert.ToInt32(s)).ToArray();
                return new Point4D(a[0], a[1], a[2], a[3]);
            });
        }

        [TestCase("Day25.txt", 394)]
        public void Test(string file, int count)
        {
            var pl = Read(file).ToArray();

            // search for neigbours
            for ( int a = 0; a < pl.Length; a++ )
            {
                for ( int b = 0; b < a; b++ )
                {
                    var pa = pl[a];
                    var pb = pl[b];
                    if (Castellation.Distance(pa,pb) <= Castellation.MaxDistance)
                    {
                        pa.Neighbours.Add(pb);
                        pb.Neighbours.Add(pa);
                    }
                }
            }

            // ready to count castellations
            var pointsToProcess = new HashSet<Point4D>(pl);
            var castellations = new List<HashSet<Point4D>>();
            while ( pointsToProcess.Any() )
            {
                var point = pointsToProcess.First();
                var castellation = Castellation.DefineCastellation(point);
                castellations.Add(castellation);
                foreach( var p in castellation )
                {
                    pointsToProcess.Remove(p);
                }
            }
            Assert.AreEqual(count, castellations.Count);
        }
    }
}
