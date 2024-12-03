using NUnit.Framework;
using NUnit.Framework.Legacy;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

namespace AdventOfCode2018.Day3
{
    public class Detector
    {
        public const int Overlapped = -1;

        private readonly IDictionary<(int, int), int> _map = new Dictionary<(int, int), int>();

        public void Add(Claim c)
        {
            for (int x = c.Left; x < c.Right; x++)
            {
                for (int y = c.Top; y < c.Bottom; y++)
                {
                    Mark(x, y, c.Id);
                }
            }
        }

        public void Mark(int x, int y, int id)
        {
            var key = (x, y);
            if (_map.ContainsKey(key))
            {
                _map[key] = Overlapped;
            }
            else
            {
                _map.Add(key, id);
            }
        }

        public int Count(int id = Overlapped)
        {
            return _map.Values.Where(v => v == id).Count();
        }
    }

    public record Rectangle(int Left, int Top, int Width, int Height)
    {
        /// The number of inches between the left edge of the fabric and the left edge of the rectangle.
        /// The number of inches between the top edge of the fabric and the top edge of the rectangle.
        /// The width of the rectangle in inches.
        /// The height of the rectangle in inches.
        public int Right => Left + Width;
        public int Bottom => Top + Height;
        public int Area => Height * Width;

        public (int X, int Y)? Intersection(int x1, int y1, int x2, int y2)
        {
            var x = Math.Max(x1, x2);
            var y = Math.Min(y1, y2);
            return y - x > 0 ? (x, y) : null;
        }

        public Rectangle IntersectionSquare(Rectangle other)
        {
            var x = Intersection(Left, Right, other.Left, other.Right);
            if (!x.HasValue) return null; // no intersection by x
            var y = Intersection(Top, Bottom, other.Top, other.Bottom);
            if (!y.HasValue) return null; // no intersection by y
            return new Rectangle((int)x?.X, (int)y?.X, (int)(x?.Y - x?.X), (int)(y?.Y - y?.X));
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

        public static IEnumerable<Rectangle> Intersections(IList<Rectangle> list)
        {
            var intersections = new List<Rectangle>();
            for (int i = 0; i < list.Count; i++)
            {
                for (int j = 0; j < i; j++)
                {
                    var intersection = list[i].IntersectionSquare(list[j]);
                    if (intersection != null)
                        intersections.Add(intersection);
                }
            }
            return intersections;
        }
    }

    public record Claim : Rectangle
    {
        public int Id { get; set; }
        public Claim(int left, int top, int width, int height)
            : base(left, top, width, height) { }
    }

    public class Day3
    {

        /// #123 @ 3,2: 5x4
        /// means that claim ID 123 specifies a rectangle 
        /// 3 inches from the left edge, 2 inches from the top edge, 5 inches wide, and 4 inches tall.
        public Claim ParseClaim(string text)
        {
            var pattern = @"#([0-9]+)\s@\s([0-9]+),([0-9]+):\s([0-9]+)x([0-9]+)";
            var matches = Regex.Matches(text, pattern);
            foreach (Match match in matches)
            {
                return new Claim(
                    Convert.ToInt32(match.Groups[2].Value),
                    Convert.ToInt32(match.Groups[3].Value),
                    Convert.ToInt32(match.Groups[4].Value),
                    Convert.ToInt32(match.Groups[5].Value))
                {
                    Id = Convert.ToInt32(match.Groups[1].Value)
                };
            }
            throw new ArgumentException("Seems format is different from expected", nameof(text));
        }
    }

    public class Day3Test
    {
        private readonly Day3 day3 = new Day3();

        [Test]
        public void TestParsing()
        {
            const string text = "#123 @ 3,2: 5x4";
            // means that claim ID 123 specifies a rectangle 
            // 3 inches from the left edge, 2 inches from the top edge, 5 inches wide, and 4 inches tall.
            var claim = day3.ParseClaim(text);
            ClassicAssert.AreEqual(123, claim.Id, "Id is wrong");
            ClassicAssert.AreEqual(3, claim.Left, "Left is wrong");
            ClassicAssert.AreEqual(2, claim.Top, "Top is wrong");
            ClassicAssert.AreEqual(5, claim.Width, "Width is wrong");
            ClassicAssert.AreEqual(4, claim.Height, "Height is wrong");
        }

        [TestCase("#1 @ 1,3: 4x4", "#2 @ 3,1: 4x4", 4)]
        public void TestIntersection(string claim1, string claim2, int expected)
        {
            var c1 = day3.ParseClaim(claim1);
            var c2 = day3.ParseClaim(claim2);
            var calculated = c1.IntersectionSquare(c2).Area;
            ClassicAssert.AreEqual(expected, calculated, "Wrong intersection area");
            var detector = new Detector();
            detector.Add(c1);
            detector.Add(c2);
            ClassicAssert.AreEqual(expected, detector.Count(), "Wrong intersection area calculated by detector");
        }

        [TestCase("Day03/input.txt")]
        public void TestFile(string file)
        {
            var lines = File.ReadAllLines(Path.Combine(App.Directory, file));
            var detector = new Detector();
            var claims = lines.Select(l => day3.ParseClaim(l)).ToArray();
            foreach (var claim in claims)
            {
                detector.Add(claim);
            }
            ClassicAssert.AreEqual(97218, detector.Count(), "Number of intersections"); // task 1

            // task 2 
            var set = new HashSet<int>();
            for (int i = 0; i < claims.Length; i++)
            {
                // is this is the claim that does not intersect with any else?
                var claim = claims[i];
                for (int j = 0; j < claims.Length; j++)
                {
                    if (j == i) continue;
                    if (claim.IntersectionSquare(claims[j]) != null)
                    {
                        claim = null;
                        break;
                    }
                }

                if (claim != null)
                {
                    set.Add(claim.Id);
                }
            }
            ClassicAssert.AreEqual(717, set.Single(), "Not overlapping area");
        }
    }
}