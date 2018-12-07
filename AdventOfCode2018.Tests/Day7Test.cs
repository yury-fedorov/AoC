using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using NUnit.Framework;
using System.Text.RegularExpressions;

// {}
// []
namespace AdventOfCode2018.Tests
{

    public class Day7Test
    {
        // A,B
        (char,char) ParseLine(string line) {
            var pattern = @"Step ([A-Z]) must be finished before step ([A-Z]) can begin.";
            var matches = Regex.Matches(line, pattern);
            foreach (Match match in matches)
            {
                return (
                    match.Groups[1].Value.Single(), // first
                    match.Groups[2].Value.Single() );
            }
            throw new ArgumentException(line);

        }


        public string t1(IEnumerable<string>lines) {
            var dpl = lines.Select(l => ParseLine(l))
                .OrderBy( a=> string.Concat<char>( new[] { a.Item1, a.Item2 } ) )
                .ToList();
            var set = dpl.Select(a=>a.Item1).Concat(dpl.Select(a=>a.Item2)).ToHashSet();

            var r = new List<char>();

            while (set.Any()) {
                // all elements without dependency from left ordered by alphabet
                var set1 = set.Where(e => !dpl.Any(t => t.Item2 == e))
                    .OrderBy(a => a).ToArray();
                foreach(var e1 in set1)
                {
                    var dpl1 = dpl.Where(a => a.Item1 != e1 ).ToList();
                    var isInternalDependency = dpl1.Any(t => t.Item2 == e1);
                    if (isInternalDependency) continue;
                    r.Add(e1);
                    set.Remove(e1); // this element not necessary any more
                    dpl = dpl1;
                    break;
                }
            }

            return string.Concat(r); 
        }

        [TestCase("Day7Sample.txt", "CABDFE")]
        [TestCase("Day7Input.txt", "BGJCNLQUYIFMOEZTADKSPVXRHW")]
        public void TestCase1(string file, string er1) {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file));
            var r1 = t1(lines);
            Assert.AreEqual(er1,r1, "wrong result 1");
        }
    }
}
// {}
// []