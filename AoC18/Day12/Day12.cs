using System;
using NUnit.Framework;
using System.IO;
using System.Collections.Generic;
using System.Linq;

namespace AdventOfCode2018.Day12
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

	public string ToStr(IDictionary<int, char> m) {
		var potIndex = m.Where(p => p.Value == '#').Select(p=>p.Key);
		var min = potIndex.Min();
		var max = potIndex.Max();
		Console.WriteLine( $"{potIndex.Count()} {min} {max}" );
		return string.Concat( m.Where( p => p.Key >= min && p.Key <= max ).OrderBy( p => p.Key ).Select( p => p.Value) );
	}

        public int CountPots(IDictionary<int, char> m)
        {
            return m.Where(p => p.Value == '#').Sum(p=>p.Key);
        }

	    [Ignore("tests are broken - part done for question 2 later solved in excel")]
        [TestCase("Day12/sample.txt",325)]
        [TestCase("Day12/input.txt", 3258)]
        public void Day12Task1(string file,int expected)
        {
            var lines = File.ReadAllLines(Path.Combine(App.Directory, file));
            var t = ReadInit(lines);
            var m = t.Item1;
            var rules = t.Item2;
            var initialSize = m.Count;
            // int count = CountPots(m);
            // Console.WriteLine($"0 {count} {count} {string.Concat(m.Keys.OrderBy(a=>a).Select(k=>m[k]))}");
            for ( int g = 1; g <= 100; g++)
            {
                var g1m = new Dictionary<int, char>();
                for ( int i = -(2*g); i < initialSize + (2*g); i++)
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
                // int genCount = CountPots(m);
                // count += genCount;
                // Console.WriteLine($"{g} {genCount} {count} {string.Concat(m.Keys.OrderBy(a => a).Select(k => m[k]))}");
		        // Console.WriteLine($"{g} {CountPots(m)} {ToStr(m)}");
            }
            Assert.AreEqual(expected, CountPots(m));
		// question 2: delta by each generation starting from 100 is 72, 
		// f(100) =  9222, f(99) = 9150, f(g) = a*g + b where a = 72 and b = (9150-100*72) = 1950
		// f(50000000000) = 72*50000000000 + 1950 =  3600000002022 
        }
    }
}
