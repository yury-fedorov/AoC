using System;
using System.IO;
using System.Linq;
using NUnit.Framework;
namespace AdventOfCode2018.Tests
{
    public class Day4Test
    {
        private readonly Day4.Day4 day4 = new Day4.Day4();

        [TestCase("Day4Sample.txt", 10, 24, 99, 45)]
        [TestCase("Day4Input.txt", 1993, 36, 2137, 50)]
        public void TestStar1(string file, int guardId, int minute, int guard2, int minute2)
        {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file));
            var log = day4.ReadLog(lines).ToArray();
            var map = day4.GuardSleep(log);
            var sleptMost = map.Values.Max();
            var id = map.Where(p => p.Value == sleptMost).Select(p => p.Key).Single();
            Assert.AreEqual(guardId, id, "wrong guard id");
            var minuteCalculated = day4.MostSleptMinutes(log, id).Item2.Single();
            Assert.AreEqual(minute, minuteCalculated, "wrong minute");

            var guards = map.Keys;
            var guardMostSleptMinute = guards.Select(g => Tuple.Create(g, day4.MostSleptMinutes(log, g)))
                .ToArray();
            var minute2c = guardMostSleptMinute.Max(t => t.Item2.Item1);
            var guard2c = guardMostSleptMinute.Where(t => t.Item2.Item1 == minute2c).Single();
            Assert.AreEqual(guard2, guard2c.Item1, "wrong guard");
            Assert.AreEqual(minute2, guard2c.Item2.Item2.Single(), "wrong minute");
            // Assert.AreEqual(0, guard2 * minute2, "response 2");
        }
    }
}
