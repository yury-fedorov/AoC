using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Linq;

namespace AdventOfCode2018.Day20
{
    public class Room
    {
        public readonly (int,int) Position;
        public readonly IDictionary<(int,int),Room> Conections = new Dictionary<(int,int),Room>();
        public Room( (int,int) position) { Position = position; }
    }

    public class Map
    {
        readonly IDictionary<(int, int), List<(int, int)>> _roomNeigbours 
            = new Dictionary<(int, int), List<(int, int)>>();

        public void AddDoor( (int,int) a, (int,int) b)
        {
            Neigbours(a).Add(b);
            Neigbours(b).Add(a);
        }

        public List<(int,int)> Neigbours((int,int) room)
        {
            List<(int, int)> list;
            if (!_roomNeigbours.TryGetValue(room, out list)) 
            {
                list = new List<(int, int)>();
                _roomNeigbours.Add(room, list);
            }
            return list;
        }

        public ((int,int),(int,int)) Bounds() {
            var minX = _roomNeigbours.Keys.Min(a => a.Item1);
            var minY = _roomNeigbours.Keys.Min(a => a.Item2);
            var maxX = _roomNeigbours.Keys.Max(a => a.Item1);
            var maxY = _roomNeigbours.Keys.Max(a => a.Item2);
            return ( (minX,minY), (maxX,maxY) );
        }
    }

    public class Day20
    {
        public static (int, int) ToDirection(char direction)
        {
            int dx = 0;
            int dy = 0;
            switch (direction)
            {
                case 'E': dx =  1; break;
                case 'W': dx = -1; break;
                case 'N': dy = -1; break;
                case 'S': dy =  1; break;
                default: throw new Exception("unknown direction");
            }
            return (dx, dy);
        }

        public string [] FlatVariations(string path)
        {
            return path.Split('|');
        }

        [TestCase("^ENWWW(NEEE|SSE(EE|N))$")]
        public void Test(string path)
        {
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

            Assert.Fail("to be implemented");
        }
    }
}
