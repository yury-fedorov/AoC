using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace AdventOfCode2018.Day01 {
    public class Day1
    {
        public IEnumerable<int> ParseSequence(string frequencyChanges, char separator)
            => frequencyChanges.Split(separator).Select(a => Convert.ToInt32(a.Trim()));

        // first task
        public int CalculateFrequency(string frequencyChanges, char separator = ',')
            => ParseSequence(frequencyChanges, separator).Sum();

        public int FirstRepetition(string frequencyChanges, char separator)
        {
            var sequance = ParseSequence(frequencyChanges, separator).ToArray();
            var set = new HashSet<int>();
            var sum = 0;
            while (true)
            {
                var curSequance = sequance;
                foreach (var delta in curSequance)
                {
                    set.Add(sum);
                    sum += delta;
                    if (set.Contains(sum))
                        return sum;
                }
            }
        }

        [TestCase("+1, -2, +3, +1",3)]
        [TestCase("+1, +1, +1", 3)]
        [TestCase("+1, +1, -2", 0)]
        [TestCase("-1, -2, -3", -6)]
        public void TestCases(string frequencyChanges, int expectedOutput) {
            var result = CalculateFrequency(frequencyChanges);
            Assert.AreEqual(expectedOutput, result, "Wrong result");
        }
		
        [TestCase("Day01/input.txt")]
        public void Task1(string file) {
            var frequencies = File.ReadAllText(Path.Combine(App.Directory,file));
             var result = CalculateFrequency(frequencies, '\n');
            Assert.AreEqual(500, result, "answer 1");
        }

        [TestCase("+1,-2,+3,+1",2)]
        [TestCase("+1, -1",0)]
        [TestCase("+3, +3, +4, -2, -4", 10)]
        [TestCase("-6, +3, +8, +5, -6",5)]
        [TestCase("+7, +7, -2, -7, -4",14)]
        public void TestTask2(string sequence, int expected) {
             var result = FirstRepetition(sequence, ',');
            Assert.AreEqual(expected, result, "test 2");
        }

        [TestCase("Day01/input.txt")]
        public void Task2(string file) {
            var frequencies = File.ReadAllText(Path.Combine(App.Directory,file));
             var result = FirstRepetition(frequencies, '\n');
            Assert.AreEqual(709, result, "answer 2");
        }
    }
}