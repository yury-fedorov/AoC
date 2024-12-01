using System.IO;
using System.Linq;
using NUnit.Framework;
using NUnit.Framework.Legacy;

namespace AoC17
{
    public class Day04
    {
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
            ClassicAssert.AreEqual(expected, IsValid(line));
        }

        [TestCase("abcde fghij", true)]
        [TestCase("abcde xyz ecdab", false)]
        public void Sample2(string line, bool expected)
        {
            ClassicAssert.AreEqual(expected, IsValid2(line));
        }

        [TestCase("Day04Input.txt", 386, 208)]
        public void Test1(string file, int expected, int expected2) {
            var lines = File.ReadAllLines( Path.Combine( App.Directory, file ) );
            ClassicAssert.IsNotEmpty(lines);
            ClassicAssert.AreEqual(expected, lines.Count(IsValid));
            ClassicAssert.AreEqual(expected2, lines.Count(IsValid2));
        }
    }
}
