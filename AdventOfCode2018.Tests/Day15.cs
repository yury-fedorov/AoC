using System;
using System.Collections.Generic;
using System.Linq;

namespace AdventOfCode2018.Day15
{
    public class Point : Tuple<int,int>
    {
        public Point(int x, int y) : base (x,y) { }
        public int X => Item1;
        public int Y => Item2;

        public int Distance(Point other) => Math.Abs(X - other.X) + Math.Abs(Y - other.Y);

        public Point Go(Direction direction) {
            var dx = direction == Direction.Left ? -1 : direction == Direction.Right ? 1 : 0;
            var dy = direction == Direction.Up   ? -1 : direction == Direction.Down  ? 1 : 0;
            return new Point(X + dx, Y + dy);
        }
    }

    public enum Direction { Left, Right, Up, Down }

    public enum Race { Elf, Goblin }

    public class Man
    {
        public int Id; // debug and identification purposes
        public Race Race => Id > 0 ? Race.Elf : Id < 0 ? Race.Goblin : throw new Exception("unexpected race");
        public int HitPoints; // left hit points
        public bool IsAlive => HitPoints > 0;
    }

    public class MapGuide
    {
        char[,] Map;
        public const char Wall = '#';
        public const char Path = '.';
        public const char Goblin = 'G';
        public const char Elf = 'E';

        public readonly HashSet<Direction> AllDirections
             = Enum.GetValues(typeof(Direction)).Cast<Direction>().ToHashSet();

        public char At(Point point)
            => (point.X < 0 || point.X >= Map.GetLength(0))
            ? Wall
            : (point.Y < 0 || point.Y >= Map.GetLength(1))
            ? Wall
            : Map[point.X, point.Y];

        public IEnumerable<Direction> Directions(Point start)
        {
            // in which directions we may go from here?
            return AllDirections.Where( d => At(start.Go(d)) == Path );
        }
    }

    public class Combat {
        readonly List<Man> men = new List<Man>();
        public IEnumerable<Man> MenOfRace(Race race) => men.Where(m => m.Race == race && m.IsAlive);
        public Race Anemy(Race race) => race == Race.Elf ? Race.Goblin : Race.Elf;

        
    }

    public class Day15
    {
    }
}
