using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
namespace AdventOfCode2018.Day20 {
    public class Room {
        public readonly (int,int) Position;
        // ?
        public readonly IDictionary<(int,int),Room> Conections = new Dictionary<(int,int),Room>();
        // ?
        public Room( (int,int) position) { Position = position; }
    }

    public class Map
    {
        readonly IDictionary<(int, int), List<(int, int)>> _roomNeigbours 
            = new Dictionary<(int, int), List<(int, int)>>();

        public IEnumerable<(int, int)> Doors() => _roomNeigbours.Keys;

        public void AddDoor( (int,int) a, (int,int) b) {
            Neigbours(a).Add(b);
            Neigbours(b).Add(a);
        }

        public List<(int,int)> Neigbours((int,int) room) {
            if (!_roomNeigbours.TryGetValue(room, out var list)) {
                list = new List<(int, int)>();
                _roomNeigbours.Add(room, list);
            }
            return list;
        }
        // ?
        public ((int,int),(int,int)) Bounds() {
            var minX = _roomNeigbours.Keys.Min(a => a.Item1);
            var minY = _roomNeigbours.Keys.Min(a => a.Item2);
            var maxX = _roomNeigbours.Keys.Max(a => a.Item1);
            var maxY = _roomNeigbours.Keys.Max(a => a.Item2);
            return ( (minX,minY), (maxX,maxY) );
        }
    }

    public interface IPart {
        int Length();
        IEnumerable<char> Path();
    }

    // ? 
    class Door : IPart {
        public readonly char _door;
        public Door(char door) { _door = door; }
        public int Length() => 1;
        public IEnumerable<char> Path() => new[] { _door };
    }

    public class Sequence : IPart {
        public readonly ICollection<IPart> _sequence = new LinkedList<IPart>();
        public int Length() => _sequence.Sum(p=>p.Length());
        public IEnumerable<char> Path() => _sequence.SelectMany(p => p.Path());
    }

    class Option : IPart
    {
        public readonly ICollection<IPart> _alternatives = new List<IPart>();
        string optimized = null;
        string Optimize()
        {
            if (optimized == null)
            {
                var minimized = _alternatives
                    .Select(p => string.Concat(Day20.Minimize(p.Path()))).ToArray();
                var maxLength = minimized.Max(x => x.Length);
                var longest = minimized.First(x => x.Length == maxLength);
                optimized = longest;
            }
            return optimized;
        }

        public int Length() => Optimize().Length;
        public IEnumerable<char> Path() => Optimize();
    }

    public class Day20
    {
        static readonly IDictionary<string, string> _optimizations = new Dictionary<string, string>();

        public static IEnumerable<char> Minimize(IEnumerable<char> path)
        {
            var strPath = string.Concat(path);
            string optimized;

            if ( _optimizations.TryGetValue(strPath, out optimized ) )
            {
                return optimized;
            }

            // direction and point where you arrive
            var exPath = new List<(char, (int, int))>();
            int x = 0;
            int y = 0;
            exPath.Add( ('^', (0, 0)) );
            foreach ( char d in strPath)
            {
                var (dx, dy) = ToDirection(d);
                x += dx;
                y += dy;
                var cp = (x, y);
                var lastToKeep = exPath.FindIndex(z => z.Item2 == cp);
                if ( lastToKeep >= 0 )
                {
                    // a loop found! we have to remove everything in the list till this position
                    var firstIndexToRemove = lastToKeep + 1;
                    var count = exPath.Count() - firstIndexToRemove;
                    exPath.RemoveRange(firstIndexToRemove, count);
                } else
                {
                    exPath.Add( (d, cp) );
                }
            }

            optimized = string.Concat( exPath.Select(z => z.Item1).Skip(1) );
            _optimizations.Add(strPath, optimized);
            return optimized;
        }

        public static (int, int) ToDirection(char direction ) =>
            direction switch {
                 'E' => (  1, 0 ),
                 'W' => ( -1, 0 ),
                 'N' => ( 0, -1 ),
                 'S' => ( 0,  1 ),
                _ => throw new Exception("unknown direction")
            };

        public string[] FlatVariations(string path) => path.Split('|');

        const string FileName = "Day20.txt";

        [TestCase(FileName, 3788)] // answer 1
        public void Part1Test(string file, int expectedLength)
        {
            var path = File.ReadAllText(Path.Combine(App.Directory, file));
            Assert.AreEqual(expectedLength, Part1(path), "answer 1" );
        }

        [TestCase("^WNE$", 3)]
        [TestCase("^ENWWW(NEEE|SSE(EE|N))$", 10)]
        [TestCase("^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$", 18)]
        [TestCase("^ESSWWN(E|NNENN(EESS(WNSE|)SSS|WWWSSSSE(SW|NNNE)))$", 23)]
        [TestCase("^WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWS(E|SS))))$", 31)]
        public void Test(string path, int expectedLength)
            => Assert.AreEqual(expectedLength, Part1(path), "test part 1");

        public static int Part1(string path)
        {
            var stackSeq = new Stack<Sequence>();
            var stackOpt = new Stack<Option>();
            var seq = new Sequence();
            Option option = null;

            for (var i = 0; i < path.Length; i++)
            {
                char ch = path[i];
                if ("EWNS".Contains(ch))
                {
                    seq._sequence.Add(new Door(ch));
                }
                else if (ch == '(')
                {
                    // this means that the previous sequence is over
                    stackSeq.Push(seq);
                    stackOpt.Push(option);
                    option = new Option();
                    seq._sequence.Add(option);
                    seq = new Sequence();
                    option._alternatives.Add(seq);
                }
                else if (ch == ')')
                {
                    seq = stackSeq.Pop();
                    option = stackOpt.Pop();
                }
                else if (ch == '|')
                {
                    seq = new Sequence();
                    option._alternatives.Add(seq);
                }
            }
            return seq.Length();
        }

        [TestCase(FileName, 8568)] // answer 2
        public void Part2(string file, int answer2)
        {
            var path = File.ReadAllText(Path.Combine(App.Directory, file));
            var p = (0, 0);
            var map = new Map();
            var stack = new Stack<(int, int)>();
            for (var i = 0; i < path.Length; i++)
            {
                char ch = path[i];
                if ("EWNS".Contains(ch))
                {
                    // direction
                    var (dx,dy) = ToDirection(ch);
                    var p0 = p;
                    var (x, y) = p;
                    p = (x + dx, y + dy);
                    map.AddDoor(p0, p); 
                } 
                else if ( ch == '(')
                {
                    stack.Push(p);
                } 
                else if ( ch == ')')
                {
                    p = stack.Pop();
                } 
                else if ( ch == '|') 
                {
                    p = stack.Peek(); 
                }
            }
            // now all doors are saved

            var doors = map.Doors().ToArray();
            var mapDistance = new Dictionary<(int, int), int>();

            var distance = 0;
            mapDistance.Add( (0,0),distance);
            while ( mapDistance.Count() < doors.Length )
            {
                var d1 = distance + 1;
                foreach ( var prevLevelPoint in mapDistance.Where( a => a.Value == distance ).ToArray() )
                {
                    var list = map.Neigbours( prevLevelPoint.Key );                    
                    foreach (var next in list)
                    {
                        if (!mapDistance.ContainsKey(next))
                        {
                            // room was never yet visited
                            var (x1, y1) = next;
                            mapDistance.Add((x1, y1), d1);
                        }
                    }
                }
                distance++;
            }
            Assert.AreEqual(answer2, mapDistance.Values.Count(d=>d>=1000), "answer 2");
        }
    }
}