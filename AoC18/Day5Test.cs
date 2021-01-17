using System;
using System.IO;
using System.Linq;
using NUnit.Framework;
namespace AdventOfCode2018.Tests
{
    public class Day5Test
    {
        private readonly Day5.Day5 day5 = new Day5.Day5();

        // [Ignore("takes 25 seconds")]
        [TestCase("Day5Input.txt")]
        public void FileTest(string file)
        {
            var lines = File.ReadAllLines(Path.Combine(App.Directory, file));
            var polimer = lines.First();
            Assert.IsNotNull(polimer, "polimer not read");
            var output = day5.OptimizePolimer(polimer);
            // File.WriteAllText("/Users/fedoy/projects/AoC18/Day5-1.txt", output);
            Assert.AreEqual(9822,output.Length, "result1");

            var d = polimer.Select(c => Char.ToLower(c))
                .Distinct().ToDictionary(c=>c,c=> day5.OptimizePolimer(
                    String.Concat( polimer.Where(a=>Char.ToLower(a) != c ) ) ).Length );
            var min = d.Values.Min();
            Assert.AreEqual(5726, min,"result2");
        }

        [TestCase("aA","")]
        [TestCase("abBA", "")]
        [TestCase("abAB", "abAB")]
        [TestCase("aabAAB", "aabAAB")]
        [TestCase("dabAcCaCBAcCcaDA", "dabCBAcaDA")]

        public void TestCase(string polimer, string optimized)
        {
            Assert.AreEqual(optimized, day5.OptimizePolimer(polimer), "not well optimized");
        }
    }
}
