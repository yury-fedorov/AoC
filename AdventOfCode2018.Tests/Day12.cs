using System;
using NUnit.Framework;
using System.IO;
using System.Collections.Generic;
using System.Linq;

namespace AdventOfCode2018.Tests
{
    public class Rule
    {
        public readonly string Sequence; 
        public readonly bool Create; 

        public Rule(string rule)
        {
            Sequence = rule.Substring(0, 5);
            Create = rule.EndsWith(" => #");
        }

        public bool Match( IDictionary<int,char> map, int index ) {
            char[] set = new char[5];
            var zero = index - 2;
            for ( int i = 0; i < 5; i++)
            {
                set[i] = map.ContainsKey(zero + i) ? map[zero + i] : '.';
            }
            return Sequence == string.Concat(set);
        }

        public bool Apply(IDictionary<int, char> map, int index)
        {
            var symbol = Create ? '#' : '.';
            if (map.ContainsKey(index))
                map[index] = symbol;
            else
                map.Add(index, symbol);
            return Create;
        }
    }

    public class Day12
    {
        public IDictionary<int,char> MapState(string state)
        {
            var map = new Dictionary<int, char>();
            for(int i = 0; i<state.Length;i++)
            {
                map.Add(i, state[i]);
            }
            return map;
        }

        public Tuple<IDictionary<int,char>,List<Rule>> ReadInit(string[] lines)
        {
            var firstLine = lines[0];
            var map = MapState(firstLine.Replace("initial state: ", ""));
            var rules = new List<Rule>();
            for (int i = 1; i < lines.Length; i++)
            {
                var l = lines[i];
                if (l.Trim().Length > 5)
                    rules.Add(new Rule ( l.Trim() ));
            }
            return Tuple.Create(map, rules);
        }

        public int CountPots(IDictionary<int, char> m)
        {
            var potIndex = m.Where(p => p.Value == '#').Select(p=>p.Key).OrderBy(i=>i).ToArray();
            if (potIndex.Any())
            {
                var min = potIndex.First();
                var max = potIndex.Last();
                return max - min + 1;
            }
            return 0;
        }

        [TestCase("Day12Sample.txt")]
        // [TestCase("Day12Input.txt")]
        public void Test1(string file)
        {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file));
            var t = ReadInit(lines);
            var m = t.Item1;
            var rules = t.Item2;
            var initialSize = m.Count;
            int count = CountPots(m);
            Console.WriteLine($"0 {count} {count} {string.Concat(m.Keys.OrderBy(a=>a).Select(k=>m[k]))}");
            for ( int g = 1; g <= 20; g++)
            {
                var g1m = new Dictionary<int, char>();
                for ( int i = -(20); i < initialSize + (20); i++)
                {
                    g1m.Add(i, '.');
                    foreach( var r in rules)
                    {
                        if (r.Match(m, i)) {
                            r.Apply(g1m, i);
                            break; // rule found
                        };
                    }
                }
                m = g1m;
                int genCount = CountPots(m);
                count += genCount;
                Console.WriteLine($"{g} {genCount} {count} {string.Concat(m.Keys.OrderBy(a => a).Select(k => m[k]))}");

            }
            Assert.AreEqual(0, count);
        }
    }
}
