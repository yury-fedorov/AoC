using System;
using System.Linq;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Security.Cryptography.X509Certificates;
using System.Text.RegularExpressions;

namespace AdventOfCode2018
{
    namespace Day3 {

        public class Detector
        {
            public const int Overlapped = -1;

            private readonly IDictionary<Tuple<int, int>, int> _map = new Dictionary<Tuple<int, int>, int>();

            public void Add(Claim c)
            {
                for (int x = c.Left; x < c.Right; x++ )
                {
                    for (int y = c.Top; y < c.Bottom; y++)
                    {
                        Mark( x, y, c.Id );
                    }
                }
            }

            public void Mark(int x, int y, int id)
            {
                var key = Tuple.Create(x, y);
                if (_map.ContainsKey(key))
                {
                    _map[key] = Overlapped;
                }
                else
                {
                    _map.Add( key, id );
                }
            }

            public int Count(int id = Overlapped)
            {
                return _map.Values.Where(v => v == id).Count();
            }
        }
          
        public class Rectangle : Tuple <int,int,int,int>{

            public Rectangle(int left, int top, int width, int height ) 
                : base(left,top,width,height) { }

            /// The number of inches between the left edge of the fabric and the left edge of the rectangle.
            public int Left => Item1;

            /// The number of inches between the top edge of the fabric and the top edge of the rectangle.
            public int Top => Item2;

            /// The width of the rectangle in inches.
            public int Width => Item3;

            /// The height of the rectangle in inches.
            public int Height => Item4;

			public int Right { get => Left + Width; }

            public int Bottom { get => Top + Height; }

            public int Area {get => Height*Width;}

            public Tuple<int,int> Intersection( int x1, int y1, int x2, int y2) {
                var x = Math.Max(x1,x2);
                var y = Math.Min(y1,y2);
                return y - x > 0 ? Tuple.Create(x,y) : null;
            }

            public Rectangle IntersectionSquare( Rectangle other ) {
                var x = Intersection(Left,Right,other.Left,other.Right);
                if (x == null) return null; // no intersection by x
                var y = Intersection(Top, Bottom,other.Top, other.Bottom);
                if (y == null) return null; // no intersection by y
                return new Rectangle( x.Item1, y.Item1, 
                    x.Item2 - x.Item1, y.Item2 - y.Item1 );
            }
            
			public static IEnumerable<Rectangle> UnifiedCoverage(Rectangle a, Rectangle b) 
			{
				if (a.Equals(b)) return new[] { a };
				if (a.IsInside(b)) return new[] { a };
				if (b.IsInside(a)) return new[] { b };
				var intersection = a.IntersectionSquare(b);
				if (intersection == null) 
					return new[] { a, b };
				// TODO - coverage decomposition 
				throw new NotImplementedException();
			}

			public bool IsInside(Rectangle smaller)
			{
				return Left <= smaller.Left 
                  && Top <= smaller.Top
				  && Right >= smaller.Right
				  && Bottom >= smaller.Bottom;
			}

            public static IEnumerable<Rectangle> Intersections( IList<Rectangle> list ) {
                var intersections = new List<Rectangle>();
                for( int i = 0; i < list.Count; i++ ) {
                    for (int j = 0; j < i; j++ ) {
                        var intersection = list[i].IntersectionSquare(list[j]);
                        if (intersection != null)
                            intersections.Add(intersection);
                    }
                }
                return intersections;
            }
        }

        public class Claim : Rectangle {
            public int Id { get; set; }
            public Claim(int left, int top, int width, int height) 
                : base(left,top,width,height) {}
        }

        public class Day3 {

            /// #123 @ 3,2: 5x4
            /// means that claim ID 123 specifies a rectangle 
            /// 3 inches from the left edge, 2 inches from the top edge, 5 inches wide, and 4 inches tall.
            public Claim ParseClaim(string text) {
                var pattern = @"#([0-9]+)\s@\s([0-9]+),([0-9]+):\s([0-9]+)x([0-9]+)";
                var matches = Regex.Matches(text, pattern);
                foreach (Match match in matches) {
                    return new Claim(
                        Convert.ToInt32(match.Groups[2].Value),
                        Convert.ToInt32(match.Groups[3].Value),
                        Convert.ToInt32(match.Groups[4].Value),
                        Convert.ToInt32(match.Groups[5].Value)) { 
                        Id = Convert.ToInt32(match.Groups[1].Value)
                    };
                }
                throw new ArgumentException("Seems format is different from expected", nameof(text));
            }
        }
    }
}