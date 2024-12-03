using NUnit.Framework;
using NUnit.Framework.Legacy;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace AdventOfCode2018.Day05
{
    public class Day5
    {
        public bool IsOpposite(char a, char b)
            => a != b && char.ToLower(a) == char.ToLower(b);

        public string OptimizePolimer(string polimer)
        {
            var list = new LinkedList<char>(polimer);
            var node = list.First;
            while (node.Next != null)
            {
                char b = node.Value;
                char a = node.Next.Value;
                if (IsOpposite(a, b))
                {
                    // we destroy them
                    var newNode = node.Previous ?? node.Next.Next;
                    if (newNode == null) return string.Empty;
                    list.Remove(node.Next);
                    list.Remove(node);
                    node = newNode;
                }
                else node = node.Next;
            }
            return string.Concat(list);
        }
    }

    public class Day5Test
    {
        private readonly Day5 day5 = new Day5();

        // [Ignore("takes 25 seconds")]
        [TestCase("Day05/input.txt")]
        public void FileTest(string file)
        {
            var lines = File.ReadAllLines(Path.Combine(App.Directory, file));
            var polimer = lines.First();
            ClassicAssert.IsNotNull(polimer, "polimer not read");
            var output = day5.OptimizePolimer(polimer);
            // File.WriteAllText("/Users/fedoy/projects/AoC18/Day5-1.txt", output);
            ClassicAssert.AreEqual(9822, output.Length, "result1");

            var d = polimer.Select(c => char.ToLower(c))
                .Distinct().ToDictionary(c => c, c => day5.OptimizePolimer(
                    string.Concat(polimer.Where(a => char.ToLower(a) != c))).Length);
            var min = d.Values.Min();
            ClassicAssert.AreEqual(5726, min, "result2");
        }

        [TestCase("aA", "")]
        [TestCase("abBA", "")]
        [TestCase("abAB", "abAB")]
        [TestCase("aabAAB", "aabAAB")]
        [TestCase("dabAcCaCBAcCcaDA", "dabCBAcaDA")]

        public void TestCase(string polimer, string optimized)
            => ClassicAssert.AreEqual(optimized, day5.OptimizePolimer(polimer), "not well optimized");
    }
}
