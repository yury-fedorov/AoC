using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace AdventOfCode2018.Day17
{
    public record Point(int X, int Y)
    {
        public Point Dx(int dx) => new Point(X + dx, Y);
        public Point Dy(int dy) => new Point(X, Y + dy);
    }

    public class Line {
        public Point A;
        public Point B;
        public bool Horizontal => A.Y == B.Y;
    }

    public class Day17 {

        public Line ToLine(string line) {
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
        public class Map {
            public const char Sand = '.';
            public const char Clay = '#';
            public const char WaterSpring = '+';
            public const char FallingWater = '|';
            public const char RestWater = '~';

            public readonly Point TopLeft;
            public readonly Point BottomRight;
            public readonly Point Size;
            readonly char[,] _map;

            public Map(Point topLeft, Point bottomRight) {
                TopLeft = topLeft;
                BottomRight = bottomRight;
                Size = new Point( BottomRight.X - TopLeft.X + 1, BottomRight.Y - TopLeft.Y + 1 );
                _map = new char[Size.X,Size.Y];
                for (int x = topLeft.X; x <= bottomRight.X; x++ ) {
                    for (int y = topLeft.Y; y <= bottomRight.Y; y++) {
                        Set(new Point(x, y), Sand);
                    }
                }
            }

            public void DrawLine(Line line, char symbol) {
                var d0  = line.Horizontal ? line.A.Y : line.A.X;
                var d1a = line.Horizontal ? line.A.X : line.A.Y;
                var d1b = line.Horizontal ? line.B.X : line.B.Y;
                for (var d1 = d1a; d1 <= d1b; d1++) {
                    var p = line.Horizontal ? new Point(d1, d0) : new Point(d0, d1);
                    Set(p, symbol);
                }
            }

            public Point ToInternalMap(Point p) => new Point( p.X - TopLeft.X, p.Y - TopLeft.Y );

            public char At(Point p) {
                var ai = ToInternalMap(p);
                if (ai.X < 0 || ai.Y < 0 || ai.X >= Size.X || ai.Y >= Size.Y)
                    throw new IndexOutOfRangeException();
                return _map[ai.X, ai.Y];
            }

            public char Set(Point p, char value) {
                var ai = ToInternalMap(p);
                var prev = _map[ai.X, ai.Y];
                _map[ai.X, ai.Y] = value;
                return prev;
            }

            // for debug purpose
            public IEnumerable<string> Draw() {
                for (int y = TopLeft.Y; y <= BottomRight.Y; y++) {
                    var line = new StringBuilder();
                    for (int x = TopLeft.X; x <= BottomRight.X; x++) {
                        line.Append(At(new Point(x, y)));
                    }
                    yield return line.ToString();
                }
            }

            public int ? FindBottom(Point from) {
                for (int y = from.Y; y <= BottomRight.Y; y++) {
                    if (At(new Point(from.X, y)) == Clay) {
                        return y;
                    }
                }
                return null; // no buttom found
            }

            public ( int Left, int Right )? FindLeftRight(Point fallingBottom) {
                // todo - without checking deepness
                var left = fallingBottom.Dx(-1);
                var right = fallingBottom.Dx(1);
                for (; At(left) != Clay; left = left.Dx(-1)) {
                    if (left.X <= TopLeft.X) return null; // no left bound
                }
                for (; At(right) != Clay; right = right.Dx(1)) {
                    if (right.X >= BottomRight.X) return null; // no right bound
                }
                return (left.X + 1, right.X - 1); // clay borders to substract from both sides
            }
        }

        [TestCase("Day17/input.txt", -1)]
        [TestCase("Day17/sample.txt", 57)]
        public void Solution(string file, int answer1)
		{
			var lines = File.ReadAllLines(Path.Combine(App.Directory, file));
			var realLines = lines.Select(l => ToLine(l)).ToArray();

			var waterSpring = new Point(500, 0);

			var allPoints = realLines.SelectMany(l => new[] { l.A, l.B }).Concat(new[] { waterSpring }).ToArray();
			var minX = allPoints.Min(p => p.X);
			var minY = allPoints.Min(p => p.Y);
			var maxX = allPoints.Max(p => p.X);
			var maxY = allPoints.Max(p => p.Y);

			var map = new Map(new Point(minX, minY), new Point(maxX, maxY));
			map.Set(waterSpring, Map.WaterSpring);
			foreach (var line in realLines) {
				map.DrawLine(line, Map.Clay);
			}

			var springs = new HashSet<Point>();
            var springsDone = new HashSet<Point>();
			springs.Add(waterSpring);
			while ( springs.Any() ) {
				var spring = springs.First();
				springs.Remove(spring);
                springsDone.Add(spring);
                TraceWaterSpring(spring, map)
                    .Where( s => !springsDone.Contains(s) )
                    .ToList().ForEach( s => springs.Add(s) );
			}

			File.WriteAllLines("AoC18-Day17.txt", map.Draw());
            var countWater = map.Draw().SelectMany(l => l.ToCharArray())
                .Count(c => c == Map.RestWater || c == Map.FallingWater );

            Assert.AreEqual( answer1, countWater, "answer 1");
		}

		private static IEnumerable<Point> TraceWaterSpring(Point waterSpring, Map map) {
			// start to draw 
			// 1. detect the first bottom
			int? y = map.FindBottom(waterSpring);
			var y0 = waterSpring.Y + 1;
			var y1 = y.HasValue ? y.Value - 1 : map.BottomRight.Y;
			if (y1 <= y0) return Enumerable.Empty<Point>();

			var fallingBottom = new Point(waterSpring.X, y1);
			var fallingSpringLine = new Line { A = new Point(waterSpring.X, y0), B = fallingBottom };
			map.DrawLine(fallingSpringLine, Map.FallingWater);

            if (y == null) return Enumerable.Empty<Point>(); // that is the end, we are at the bottom

            // now we at the bottom
            // now we fill the volume with water
            var leftRight = map.FindLeftRight(fallingBottom);
			var prevLeftRight = leftRight;
			var bottomRestWaterLine = new Line { 
                A = new Point( (leftRight?.Left).Value,  fallingBottom.Y), 
                B = new Point( (leftRight?.Right).Value, fallingBottom.Y) };
			map.DrawLine(bottomRestWaterLine, Map.RestWater);

			var result = new List<Point>();

			// this bottom can be partial and origin of two new springs, to check
			// this is important to fill the part below
			for (var bottomLeft = new Point(waterSpring.X-1,fallingBottom.Y); 
				bottomLeft.X >= bottomRestWaterLine.A.X; bottomLeft = bottomLeft.Dx(-1) ) {
				if ( map.At(bottomLeft.Dy(1)) != Map.Clay ) {
					// hole on the left
					result.Add(bottomLeft);
					break; // found a spring on the left
				}
			}

			for (var bottomRight = new Point(waterSpring.X + 1, fallingBottom.Y);
				bottomRight.X <= bottomRestWaterLine.B.X; bottomRight = bottomRight.Dx(1)) {
				if (map.At(bottomRight.Dy(1)) != Map.Clay) {
					// hole on the right
					result.Add(bottomRight);
					break; // found a spring on the right
				}
			}

			// now we are ready to go up
			var falling = fallingBottom.Dy(-1);
			for (; (leftRight = map.FindLeftRight(falling)) != null; falling = falling.Dy(-1)) {
				// go outside of the cup from left
				if ( prevLeftRight?.Left > leftRight?.Left ) break;

				// go outside of the cup from right
				if ( prevLeftRight?.Right < leftRight?.Right ) break; 

				map.DrawLine(new Line { 
                    A = new Point( (leftRight?.Left).Value, falling.Y), 
                    B = new Point( (leftRight?.Right).Value, falling.Y) }, Map.RestWater);
				prevLeftRight = leftRight; // we remember we have made it more streight
			}
			// we are at the top of the cup
			// now it can fall down on both sides
			var cupLeftInternalPoint = new Point( (prevLeftRight?.Left).Value, falling.Y);
			var cupRightInternalPoint = new Point( (prevLeftRight?.Right).Value, falling.Y);

            Point fallingRightPoint = null;
            for ( int dx = 1; map.At(cupRightInternalPoint.Dx(dx)) != Map.Clay; dx++ )
            {
                if ( map.At(cupRightInternalPoint.Dx(dx+1).Dy(1)) != Map.Clay )
                {
                    fallingRightPoint = cupRightInternalPoint.Dx(dx + 1);
                    break;
                }
            }

            Point fallingLeftPoint = null;
            for (int dx = -1; map.At(cupLeftInternalPoint.Dx(dx)) != Map.Clay; dx--)
            {
                if (map.At(cupLeftInternalPoint.Dx(dx - 1).Dy(1)) != Map.Clay)
                {
                    fallingLeftPoint = cupLeftInternalPoint.Dx(dx - 1);
                    break;
                }
            }

			var line = new Line {
				A = fallingLeftPoint  ?? cupLeftInternalPoint,
				B = fallingRightPoint ?? cupRightInternalPoint
			};

			if ( map.At( new Point( waterSpring.X, falling.Y ) ) != Map.RestWater ) {
				map.DrawLine(line, Map.FallingWater);
			}

			// we detect the sides where it goes down
			if (fallingLeftPoint != null) result.Add(fallingLeftPoint);
            if (fallingRightPoint != null) result.Add(fallingRightPoint);
			return result;
		}
	}
}
