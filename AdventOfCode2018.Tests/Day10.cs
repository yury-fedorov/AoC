using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Diagnostics;
using System;
using NUnit.Framework;

// {} []
namespace AdventOfCode2018.Day10
{
    public class TwoD
    {
        public int X;
        public int Y;
    }

    public class Point : TwoD { }

    public class Velocity : TwoD { }

    public class Light
    {
        public Point point0;
        public Velocity velocity;
        public Point AtTime(int time)
        {
            return new Point
            {
                X = point0.X + (velocity.X * time),
                Y = point0.Y + (velocity.Y * time)
            };
        }
    }

    public class Rectangle
    {
        public Point min; // ie topLeft
        public Point max; // ie bottomRight
        public long Area()
        { return (max.X - min.X) * (max.Y - min.Y); }
    }

    public class Day10
    {

        // position=<-2,  2> velocity=< 2,  0>
        static Light ParseLine(string line)
        {
            const string pattern = @"position=<\s*([-0-9]+),\s*([-0-9]+)> velocity=<\s*([-0-9]+),\s*([-0-9]+)>";
            // var pattern = @"([-0-9]+),\s*([-0-9]+).*([-0-9]+),\s*([-0-9]+)";

            var matches = Regex.Matches(line, pattern);
            foreach (Match match in matches)
            {
                try
                {
                    int px = Convert.ToInt32(match.Groups[1].Value);
                    int py = Convert.ToInt32(match.Groups[2].Value);
                    int vx = Convert.ToInt32(match.Groups[3].Value);
                    int vy = Convert.ToInt32(match.Groups[4].Value);
                    return new Light
                    {
                        point0 = new Point
                        {
                            X = px, 
                            Y = py
                        },
                        velocity = new Velocity {
                            X= vx,
                            Y= vy}
                    };
                } catch (Exception e)
                {
                    // throw new Exception( match.Groups[0].Value, e);
                    throw;
                }
            }
            throw new ArgumentException(line);

        }

        static Rectangle Bounds(IEnumerable<Point> points)
        {
            var min_ = new Point
            {
                X = points.Select(p => p.X).Min(),
                Y = points.Select(p => p.Y).Min()
            };
            var max_ = new Point
            {
                X = points.Select(p => p.X).Max(),
                Y = points.Select(p => p.Y).Max()
            };
            return new Rectangle { min = min_, max = max_ };
        }


        [TestCase("Day10Sample.txt",3)]
        [TestCase("Day10Input.txt", 3)]
        public void Test(string file, long et)
        {
            var lines = File.ReadAllLines(Path.Combine(App.App.Directory, file));
            var lights = lines.Select(l => ParseLine(l)).ToArray();
            var b0 = Bounds(lights.Select(l => l.point0));
            Assert.True( b0.min.X < b0.max.X && b0.min.Y < b0.max.Y, "expected area" );
            Assert.True(b0.Area() > 0, "expected > 0");
            var areaPrev = b0.Area();
            int t = 1;
            for (; areaPrev > 0; t++)
            {
                var area = Bounds(lights.Select(l => l.AtTime(t))).Area();
                if (areaPrev <= area) break;
            }
            Assert.AreEqual(et, t, string.Format("{0}vs{1}",
                Bounds(lights.Select(l => l.AtTime(t))).Area(),
                Bounds(lights.Select(l => l.AtTime(t-1))).Area()));
        }
    }
}
// {} []
