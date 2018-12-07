using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using NUnit.Framework;
using System.Text.RegularExpressions;
using System.Diagnostics;

// {}
// []
namespace AdventOfCode2018.Tests7
{
    class Task {
        public char letter;
        public int duration;
        public int left; 
    }

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

        int t2(IEnumerable<string> lines, int deltaDuration, int workers)
        {
            var dpl = lines.Select(l => ParseLine(l))
                .OrderBy(a => string.Concat<char>(new[] { a.Item1, a.Item2 }))
                .ToList();
            var set = dpl.Select(a => a.Item1).Concat(dpl.Select(a => a.Item2)).ToHashSet();

            int time = 0;

            var inProgress = new List<Task>();

            while (set.Any())
            {
                // all elements without dependency from left ordered by alphabet
                var set1 = set.Where(e => !dpl.Any(t => t.Item2 == e))
                    .OrderBy(a => a).ToArray();

                if (inProgress.Count() < workers ) {
                    // there are empty spaces
                    // check if we may start a task to run
                    var candidates = set1.Where(c => !inProgress.Any(ip => ip.letter == c)).ToList();
                    while (candidates.Any() && inProgress.Count() < workers ) {
                        var next = candidates.First();
                        candidates.Remove(next);
                        var duration = deltaDuration + (next - 'A' + 1);
                        inProgress.Add(new Task { 
                            letter = next, 
                            duration = duration,
                            left = duration
                            });
                    }
                }

                {
                    // all workers are occupied
                    // we only update progress
                    var finished = new HashSet<char>();
                    foreach (var task in inProgress)
                    {
                        task.left -= 1;
                        if (task.left == 0)
                        {
                            // done
                            finished.Add(task.letter);
                            set.Remove(task.letter); // this element not necessary any more
                            dpl = dpl.Where(a => a.Item1 != task.letter).ToList(); // new dependencies
                        }
                    }
                    inProgress.RemoveAll(t => finished.Contains(t.letter));
                }
                
                time++;
            }

            return time;
        }

        [TestCase("Day7Sample.txt", "CABDFE", 0,2, 15)]
        [TestCase("Day7Input.txt", "BGJCNLQUYIFMOEZTADKSPVXRHW", 60,5, 1017)]
        public void TestCase1(string file, string er1, int deltaDuration, int workers, int ed) {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file));
            var r1 = t1(lines);
            Assert.AreEqual(er1,r1, "wrong result 1");
            var r2 = t2(lines, deltaDuration, workers);
            Assert.AreEqual(ed, r2, "wrong result 2");
        }
    }
}
// {}
// []