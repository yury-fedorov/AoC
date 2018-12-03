using NUnit.Framework;
using System.IO;
using AdventOfCode2018;

namespace AdventOfCode2018
{
    public class Day1Test
    {
        private readonly Day1 day1 = new Day1();

        [TestCase("+1, -2, +3, +1",3)]
        [TestCase("+1, +1, +1", 3)]
        [TestCase("+1, +1, -2", 0)]
        [TestCase("-1, -2, -3", -6)]
        public void TestCases(string frequencyChanges, int expectedOutput) {
            var result = day1.CalculateFrequency(frequencyChanges);
            Assert.AreEqual(expectedOutput, result, "Wrong result");
        }

        public const string Directory = "D:\\projects\\leonteq\\AoC18\\AdventOfCode2018.Tests";

        [TestCase("Day1-Input.txt")]
        public void Task1(string file) {
            var frequencies = File.ReadAllText(Path.Combine(Directory,file));
             var result = day1.CalculateFrequency(frequencies, '\n');
            Assert.AreEqual(500, result, "Wrong result");
        }

        [TestCase("+1,-2,+3,+1",2)]
        [TestCase("+1, -1",0)]
        [TestCase("+3, +3, +4, -2, -4", 10)]
        [TestCase("-6, +3, +8, +5, -6",5)]
        [TestCase("+7, +7, -2, -7, -4",14)]
        public void TestTask2(string sequence, int expected) {
             var result = day1.FirstRepetition(sequence, ',');
            Assert.AreEqual(expected, result, "Wrong result");
        }

        [TestCase("Day1-Task2.txt")]
        public void Task2(string file) {
            var frequencies = File.ReadAllText(Path.Combine(Directory,file));
             var result = day1.FirstRepetition(frequencies, '\n');
            Assert.AreEqual(709, result, "Wrong result");
        }
    }
}