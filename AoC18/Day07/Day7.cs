using NUnit.Framework;
using NUnit.Framework.Legacy;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

namespace AdventOfCode2018.Day07
{
    class Task
    {
        public char letter;
        public int duration;
        public int left;
    }

    public class Day7
    {
        (char A, char B) ParseLine(string line)
        {
            var pattern = @"Step ([A-Z]) must be finished before step ([A-Z]) can begin.";
            var matches = Regex.Matches(line, pattern);
            foreach (Match match in matches)
            {
                return (match.Groups[1].Value.Single(), match.Groups[2].Value.Single());
            }
            throw new ArgumentException(line);
        }

        public string Task1(IEnumerable<string> lines)
        {
            var dpl = lines.Select(l => ParseLine(l))
                .OrderBy(a => string.Concat(new[] { a.A, a.B }))
                .ToList();
            var set = dpl.Select(a => a.A).Concat(dpl.Select(a => a.B)).ToHashSet();
            var r = new List<char>();
            while (set.Any())
            {
                // all elements without dependency from left ordered by alphabet
                var set1 = set.Where(e => !dpl.Any(t => t.B == e))
                    .OrderBy(a => a).ToArray();
                foreach (var e1 in set1)
                {
                    var dpl1 = dpl.Where(a => a.A != e1).ToList();
                    var isInternalDependency = dpl1.Any(t => t.B == e1);
                    if (isInternalDependency) continue;
                    r.Add(e1);
                    set.Remove(e1); // this element not necessary any more
                    dpl = dpl1;
                    break;
                }
            }

            return string.Concat(r);
        }

        int Task2(IEnumerable<string> lines, int deltaDuration, int workers)
        {
            var dpl = lines.Select(l => ParseLine(l))
                .OrderBy(a => string.Concat(new[] { a.A, a.B }))
                .ToList();
            var set = dpl.Select(a => a.A).Concat(dpl.Select(a => a.B)).ToHashSet();

            int time = 0;

            var inProgress = new List<Task>();

            while (set.Any())
            {
                // all elements without dependency from left ordered by alphabet
                var set1 = set.Where(e => !dpl.Any(t => t.Item2 == e))
                    .OrderBy(a => a).ToArray();

                if (inProgress.Count() < workers)
                {
                    // there are empty spaces
                    // check if we may start a task to run
                    var candidates = set1.Where(c => !inProgress.Any(ip => ip.letter == c)).ToList();
                    while (candidates.Any() && inProgress.Count() < workers)
                    {
                        var next = candidates.First();
                        candidates.Remove(next);
                        var duration = deltaDuration + (next - 'A' + 1);
                        inProgress.Add(new Task
                        {
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

        [TestCase("Day07/sample.txt", "CABDFE", 0, 2, 15)]
        [TestCase("Day07/input.txt", "BGJCNLQUYIFMOEZTADKSPVXRHW", 60, 5, 1017)]
        public void Solution(string file, string answer1, int deltaDuration, int workers, int answer2)
        {
            var lines = File.ReadAllLines(Path.Combine(App.Directory, file));
            ClassicAssert.AreEqual(answer1, Task1(lines), "answer 1");
            ClassicAssert.AreEqual(answer2, Task2(lines, deltaDuration, workers), "answer 2");
        }
    }
}