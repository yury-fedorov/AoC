using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.ConstrainedExecution;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using System.Text.RegularExpressions;

namespace AdventOfCode2018.Day17
{
    public class Point : Tuple<int, int>
    {
        public Point(int x, int y) : base(x, y) {}

        public int X => Item1;
        public int Y => Item2;

        public Point Dx(int dx) => new Point(X + dx, Y);
        public Point Dy(int dy) => new Point(X, Y + dy);
    }

    public class Line
    {
        public Point A;
        public Point B;
        public bool Horizontal => A.Y == B.Y;
    }

    public class Day17 {

        static void Swap(ref int x, ref int y)
        {
            var tempswap = x;
            x = y;
            y = tempswap;
        }

        public Line ToLine(string line)
        {
            // x=309, y=1432..1458
            // y = 231, x = 423..450
            const string pattern = @"([xy])=([0-9]+), ([xy])=([0-9]+)..([0-9]+)";
            var matches = Regex.Matches(line, pattern);
            foreach (Match match in matches)
            {
                char onePointAxis = match.Groups[1].Value.First();
                int d0 = Convert.ToInt32(match.Groups[2].Value);
                char rangeAxis = match.Groups[3].Value.First();
                int d1a = Convert.ToInt32(match.Groups[4].Value);
                int d1b = Convert.ToInt32(match.Groups[5].Value);
                 
                Assert.AreNotEqual( onePointAxis, rangeAxis, "two different axis are expected" );

                int x0, y0, x1, y1;
                bool horizontalLine = onePointAxis == 'y';
                if (horizontalLine)
                {
                    y0 = y1 = d0;
                    x0 = d1a;
                    x1 = d1b;
                }
                else
                {
                    x0 = x1 = d0;
                    y0 = d1a;
                    y1 = d1b;
                }
                return new Line { A = new Point(x0, y0), B = new Point(x1,y1) };
            }
            throw new Exception("unexpected flow");
        }

        // view on the part given
        public class Map
        {
            public const char Sand = '.';
            public const char Clay = '#';
            public const char WaterSpring = '+';
            public const char FallingWater = '|';
            public const char RestWater = '~';

            public readonly Point TopLeft;
            public readonly Point BottomRight;
            public readonly Point Size;
            readonly char[,] _map;

            public Map(Point topLeft, Point bottomRight)
            {
                TopLeft = topLeft;
                BottomRight = bottomRight;
                Size = new Point( BottomRight.X - TopLeft.X + 1, BottomRight.Y - TopLeft.Y + 1 );
                _map = new char[Size.X,Size.Y];
                for (int x = topLeft.X; x <= bottomRight.X; x++ )
                {
                    for (int y = topLeft.Y; y <= bottomRight.Y; y++)
                    {
                        Set(new Point(x, y), Sand);
                    }
                }
            }


            public void DrawLine(Line line, char symbol)
            {
                var d0  = line.Horizontal ? line.A.Y : line.A.X;
                var d1a = line.Horizontal ? line.A.X : line.A.Y;
                var d1b = line.Horizontal ? line.B.X : line.B.Y;
                for (var d1 = d1a; d1 <= d1b; d1++)
                {
                    var p = line.Horizontal ? new Point(d1, d0) : new Point(d0, d1);
                    Set(p, symbol);
                }
            }

            public Point ToInternalMap(Point p)
            {
                return new Point( p.X - TopLeft.X, p.Y - TopLeft.Y );
            }

            public char At(Point p)
            {
                var ai = ToInternalMap(p);
                return _map[ai.X, ai.Y];
            }

            public char Set(Point p, char value)
            {
                var ai = ToInternalMap(p);
                var prev = _map[ai.X, ai.Y];
                _map[ai.X, ai.Y] = value;
                return prev;
            }

            public IEnumerable<string> GetStringList()
            {
                for (int y = TopLeft.Y; y <= BottomRight.Y; y++)
                {
                    var line = new StringBuilder();
                    for (int x = TopLeft.X; x <= BottomRight.X; x++)
                    {
                        line.Append(At(new Point(x, y)));
                    }
                    yield return line.ToString();
                }
            }

            public int ? FindBottom(Point from)
            {
                for (int y = from.Y; y <= BottomRight.Y; y++)
                {
                    if (At(new Point(from.X, y)) == Clay)
                    {
                        return y;
                    }
                }
                return null; // no buttom found
            }

            public Tuple<int,int> FindLeftRight(Point fallingBottom)
            {
                // todo - without checking deepness
                var left = fallingBottom.Dx(-1);
                var right = fallingBottom.Dx(1);
                for (; At(left) != Clay; left = left.Dx(-1))
                {
                    if (left.X <= TopLeft.X) return null; // no left bound
                }
                for (; At(right) != Clay; right = right.Dx(1))
                {
                    if (right.X >= BottomRight.X) return null; // no right bound
                }
                return Tuple.Create(left.X + 1, right.X - 1); // clay borders to substract from both sides
            }
        }

        [TestCase("Day17.txt")]
        public void TestSample3(string file)
        {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file));
            var realLines = lines.Select(l => ToLine(l)).ToArray();

            var waterSpring = new Point(500,0);

            var allPoints = realLines.SelectMany(l => new[] {l.A, l.B}).Concat(new []{waterSpring}).ToArray();
            var minX = allPoints.Min(p => p.X);
            var minY = allPoints.Min(p => p.Y);
            var maxX = allPoints.Max(p => p.X);
            var maxY = allPoints.Max(p => p.Y);

            var map = new Map( new Point(minX,minY), new Point(maxX,maxY) );
            map.Set(waterSpring, Map.WaterSpring);
            foreach (var line in realLines)
            {
                map.DrawLine(line, Map.Clay);
            }

            // start to draw 
            // 1. detect the first bottom
            int ? y = map.FindBottom(waterSpring);
            var y0 = waterSpring.Y + 1;
            var y1 = y.HasValue ? y.Value - 1 : map.BottomRight.Y;
            var fallingBottom = new Point(waterSpring.X, y1);
            map.DrawLine( new Line{ A = new Point(waterSpring.X,y0), B = fallingBottom }, Map.FallingWater );
            // now we at the bottom
            // now we fill the volume with water
            var leftRight = map.FindLeftRight(fallingBottom);
            var prevLeftRight = leftRight;
            map.DrawLine(new Line { A = new Point(leftRight.Item1, fallingBottom.Y), B = new Point(leftRight.Item2, fallingBottom.Y) }, Map.RestWater);
            var falling = fallingBottom.Dy(-1);
            for ( ; (leftRight = map.FindLeftRight(falling)) != null; falling = falling.Dy(-1) )
            {
                if (prevLeftRight.Item1 > leftRight.Item1) break; // go outside of the cup from left
                if (prevLeftRight.Item2 < leftRight.Item2) break; // go outside of the cup from right
                map.DrawLine(new Line { A = new Point(leftRight.Item1, falling.Y), B = new Point(leftRight.Item2, falling.Y) }, Map.RestWater);
                prevLeftRight = leftRight; // we remember we have made it more streight
            }
            // we are at the top of the cup
            // now it can fall down on both sides
            // we detect the sides where it goes down

            File.WriteAllLines( "d:\\out.txt", map.GetStringList() );
            // Assert.AreEqual(600, registers.First(), "answer 2");
        }
    }
}
