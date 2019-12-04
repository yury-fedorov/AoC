using System.IO;
using System.Linq;
using NUnit.Framework;

namespace AoC17
{
    // alt 5 - square brackets
    // alt 7 - or pipe
    // alt 8-9 - {}
    public class Day04
    {
        public const string Directory = "/Users/fedoy/projects/AoC17/AoC17";

        public bool IsValid(string line)
        {
            var words = line.Split(' ');
            return words.ToHashSet().Count == words.Length;
        }

        public bool IsValid2(string line)
        {
            var words = line.Split(' ');
            return words.Select(w => string.Concat(w.OrderBy(a => a)))
                .ToHashSet().Count == words.Length;
        }

        [TestCase("aa bb cc dd ee", true)]
        [TestCase("aa bb cc dd aa", false)]
        [TestCase("aa bb cc dd aaa", true)]
        public void Sample1(string line, bool expected)
        {
            Assert.AreEqual(expected, IsValid(line));
        }

        [TestCase("abcde fghij", true)]
        [TestCase("abcde xyz ecdab", false)]
        public void Sample2(string line, bool expected)
        {
            Assert.AreEqual(expected, IsValid2(line));
        }

        [TestCase("Day04Input.txt", 386, 208)]
        public void Test1(string file, int expected, int expected2) {
            var lines = File.ReadAllLines( Path.Combine( Directory, file ) );
            Assert.IsNotEmpty(lines);
            Assert.AreEqual(expected, lines.Count(IsValid));
            Assert.AreEqual(expected2, lines.Count(IsValid2));
        }
    }
}
