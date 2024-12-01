using NUnit.Framework;
using NUnit.Framework.Legacy;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace AdventOfCode2018.Day02
{
    public class Day2
    {

        public IDictionary<char, int> GroupLetters(string boxId)
        {
            return boxId.GroupBy(c => c) // put each character into a "bucket"
                                         // order the buckets alphabetically
                     .OrderBy(c => c.Key)
                     // then convert to dictionary where key = character, value = count
                     .ToDictionary(grp => grp.Key, grp => grp.Count());
        }

        public int CountOccurancies(IDictionary<char, int> letterToCount, int filter)
            => letterToCount.Where(a => a.Value == filter).Count();

        public Tuple<int, int> CalculateRudimentaryChecksum(string boxId)
        {
            var dictionary = GroupLetters(boxId);
            return Tuple.Create(
                CountOccurancies(dictionary, 2) > 0 ? 1 : 0,
                CountOccurancies(dictionary, 3) > 0 ? 1 : 0);
        }

        // Task 2
        // this is found by removing the differing character from either ID, 
        public string IdentifyCorrectBoxId(string a, string b)
        {
            if (a.Length != b.Length) return null;
            int? wrongIndex = null;
            for (int i = 0; i < a.Length; i++)
            {
                if (a[i] != b[i])
                {
                    // we have found a difference
                    if (wrongIndex.HasValue) return null; // more then one difference
                    wrongIndex = i;
                }
            }
            if (!wrongIndex.HasValue) return null; // perfect match is not good as well
            var list = a.ToList();
            list.RemoveAt(wrongIndex.Value);
            return string.Concat(list);
        }

        public string IdentifyCorrectBoxId(IList<string> idList)
        {
            var set = new HashSet<string>();
            for (int i = 0; i < idList.Count; i++)
            {
                for (int j = 0; j < i; j++)
                {
                    var correctId = IdentifyCorrectBoxId(idList[i], idList[j]);
                    if (correctId != null) set.Add(correctId);
                }
            }
            return set.Single();
        }

        [TestCase("abcdef", 0, 0)] // contains no letters that appear exactly two or three times.
        [TestCase("bababc", 1, 1)] // contains two a and three b, so it counts for both.
        [TestCase("abbcde", 1, 0)] // contains two b, but no letter appears exactly three times.
        [TestCase("abcccd", 0, 1)] // contains three c, but no letter appears exactly two times.
        [TestCase("aabcdd", 2, 0)] // contains two a and two d, but it only counts once.
        [TestCase("abcdee", 1, 0)] // contains two e.
        [TestCase("ababab", 0, 2)] // contains three a and three b, but it only counts once.
        public void CheckOccurancies(string boxId, int expectedTwoTimes, int expectedThreeTimes)
        {
            var result = CalculateRudimentaryChecksum(boxId);
            ClassicAssert.AreEqual(Tuple.Create(expectedTwoTimes > 0 ? 1 : 0, expectedThreeTimes > 0 ? 1 : 0), result, "Wrong checksum");
            var dictionary = GroupLetters(boxId);
            ClassicAssert.AreEqual(expectedTwoTimes, CountOccurancies(dictionary, 2), "Wrong 2 times count");
            ClassicAssert.AreEqual(expectedThreeTimes, CountOccurancies(dictionary, 3), "Wrong 3 times count");
        }

        [TestCase("Day02/sample.txt", 12)]
        [TestCase("Day02/input.txt", 5658)]
        public void Test3(string file, int expected)
        {
            var lines = File.ReadAllLines(Path.Combine(App.Directory, file));
            var result = lines.Select(line => CalculateRudimentaryChecksum(line))
                .Aggregate(Tuple.Create(0, 0), (a, b) => Tuple.Create(a.Item1 + b.Item1, a.Item2 + b.Item2));
            var checksum = result.Item1 * result.Item2;
            ClassicAssert.AreEqual(expected, checksum, "Wrong checksum");
        }

        [TestCase("fghij", "fguij", "fgij")]
        public void TestCommon(string a, string b, string ab)
        {
            var ab1 = IdentifyCorrectBoxId(a, b);
            ClassicAssert.AreEqual(ab, ab1, "Wrong correct box ID");
        }

        [TestCase("Day02/input.txt", "nmgyjkpruszlbaqwficavxneo")]
        public void Test4(string file, string commonBoxId)
        {
            var lines = File.ReadAllLines(Path.Combine(App.Directory, file));
            ClassicAssert.AreEqual(commonBoxId, IdentifyCorrectBoxId(lines), "Final result");
        }
    }
}