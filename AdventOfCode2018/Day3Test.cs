using System.Collections.Generic;
using NUnit.Framework;
using System.IO;
using System.Linq;

namespace AdventOfCode2018.Day3
{
    public class Day3Test
    {
        private readonly Day3 day3 = new Day3();

        [Test]
        public void TestParsing() {
            const string text = "#123 @ 3,2: 5x4";
            // means that claim ID 123 specifies a rectangle 
            // 3 inches from the left edge, 2 inches from the top edge, 5 inches wide, and 4 inches tall.
            var claim = day3.ParseClaim(text);
            Assert.AreEqual(123,claim.Id, "Id is wrong");
            Assert.AreEqual(3,claim.Left, "Left is wrong");
            Assert.AreEqual(2,claim.Top, "Top is wrong");
            Assert.AreEqual(5,claim.Width, "Width is wrong");
            Assert.AreEqual(4,claim.Height, "Height is wrong");
        }

        [TestCase("#1 @ 1,3: 4x4","#2 @ 3,1: 4x4",4)]
        public void TestIntersection(string claim1, string claim2, int expected) {
            var c1 = day3.ParseClaim(claim1);
            var c2 = day3.ParseClaim(claim2);
            var calculated = c1.IntersectionSquare(c2).Area;
            Assert.AreEqual(expected,calculated,"Wrong intersection area");
            var detector = new Detector();
            detector.Add(c1);
            detector.Add(c2);
            Assert.AreEqual(expected, detector.Count(), "Wrong intersection area calculated by detector");
        }

        [TestCase("Day3Input.txt")]
        public void TestFile(string file) {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory,file));
            var detector = new Detector();
            var claims = lines.Select(l => day3.ParseClaim(l)).ToArray();
            foreach (var claim in claims)
            {
                detector.Add(claim);
            }
            Assert.AreEqual(97218,detector.Count(), "Number of intersections"); // task 1

            // task 2 
            var set = new HashSet<int>();
            for (int i = 0; i < claims.Length; i++)
            {
                // is this is the claim that does not intersect with any else?
                var claim = claims[i];
                for (int j = 0; j < claims.Length; j++)
                {
                    if ( j == i ) continue;
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
            Assert.AreEqual(717,set.Single(),"Not overlapping area");
        }
    }
}