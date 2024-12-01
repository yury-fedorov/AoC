using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System;
using NUnit.Framework;
using NUnit.Framework.Legacy;

namespace AdventOfCode2018.Day10
{
    public record Point(int X, int Y) { }

    public record Velocity(int X, int Y) { }

    public record Light(Point point0, Velocity velocity)
    {
        public Point AtTime(int time) => new Point(point0.X + (velocity.X * time), point0.Y + (velocity.Y * time));
    }

    public record Rectangle (Point min, Point max) // topLeft, bottomRight
    {
	    public int Width => max.X - min.X;
	    public int Height => max.Y - min.Y;
    }

    public class Day10
    {
        // position=<-2,  2> velocity=< 2,  0>
        static Light ParseLine(string line)
        {
            const string pattern = @"position=<\s*([-0-9]+),\s*([-0-9]+)> velocity=<\s*([-0-9]+),\s*([-0-9]+)>";
            var matches = Regex.Matches(line, pattern);
            foreach (Match match in matches)
            {
                    return new Light
                    (
                        new Point
                        (
                            Convert.ToInt32(match.Groups[1].Value), 
                            Convert.ToInt32(match.Groups[2].Value)
                        ),
                        new Velocity (
                            Convert.ToInt32(match.Groups[3].Value),
                            Convert.ToInt32(match.Groups[4].Value)
                        )
                    );
            }
            throw new ArgumentException(line);
        }

        static Rectangle Bounds(IEnumerable<Point> points)
        {
            var min = new Point
            (
                points.Select(p => p.X).Min(),
                points.Select(p => p.Y).Min()
            );
            var max = new Point
            (
                points.Select(p => p.X).Max(),
                points.Select(p => p.Y).Max()
            );
            return new Rectangle( min, max );
        }


        [TestCase("Day10/sample.txt",3)]
        [TestCase("Day10/input.txt", 10003)]
        public void Test(string file, long et)
        {
            var lines = File.ReadAllLines(Path.Combine(App.Directory, file));
            var lights = lines.Select(l => ParseLine(l)).ToArray();
            var b = Bounds(lights.Select(l => l.point0));
            var yPrev = b.Height;
            int t = 0;
            for (; yPrev > 0; t++)
            {
		        var b1 = Bounds(lights.Select(l => l.AtTime(t+1)));
                var y = b1.Height;
                if (yPrev <= y) break;
		        yPrev = y;
		        b = b1;
            }
		    ClassicAssert.GreaterOrEqual( 50, b.Height );
            ClassicAssert.GreaterOrEqual( 80, b.Width );
            // Console.WriteLine( $"{b.min.X} , {b.min.Y} - {b.max.X} {b.max.Y}" );

            var ol = lights.Select(l => l.AtTime(t)).ToArray();

            for (var y = b.min.Y; y <= b.max.Y; y++)
            {
                var thisLine = ol.Where(p => p.Y == y).OrderBy(p => p.X);
                var line = new string(' ', b.Width + 1).ToCharArray();
                foreach (var p in thisLine)
                {
                    var x = p.X - b.min.X;
                    // Assert.GreaterOrEqual(x,0);
                    // Assert.Less(x, b.Width);
                    line[x] = '*';
                }
                Console.WriteLine(line);
            }
            // to write the output on the console (to see what is written)
            // sample: HI
            // my input: CZKPNARN
            // Assert.Fail();
        }
    }
}
