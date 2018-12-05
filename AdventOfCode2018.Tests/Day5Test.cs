using System;
using System.IO;
using System.Linq;
using NUnit.Framework;
namespace AdventOfCode2018.Tests
{
    public class Day5Test
    {
        private readonly Day5.Day5 day5 = new Day5.Day5();

        [TestCase("Day5Input.txt")]
        public void TestStar1(string file)
        {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file));
            Assert.IsNotNull(lines, "file not read");
        }
    }
}
