using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using NUnit.Framework;

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
        public Race Race;
        public int HitPoints; // left hit points
        public bool IsAlive => HitPoints > 0;
        public Point Position;
    }

    public class MapGuide
    {
        char[,] Map;

        public readonly int Width;
        public readonly int Height;

        public const char Wall = '#';
        public const char Path = '.';
        public const char Goblin = 'G';
        public const char Elf = 'E';

        public MapGuide(string[] lines)
        {
            Width = lines.First().Length;
            Height = lines.Length;
            Map = new char[Width, Height];
            for (int y = 0; y < Height; y++)
            {
                var line = lines[y];
                for (int x = 0; x < Width; x++)
                {
                    Map[x, y] = line[x];
                }
            }
        }

        public IEnumerable<Tuple<Race,Point>> FindAllMen() 
        { 
            for ( int x = 0; x < Width; x++)
            {
                for ( int y = 0; y < Height; y++)
                {
                    var cell = Map[x, y];
                    if ( cell == Goblin || cell == Elf)
                    {
                        var race = cell == Goblin ? Race.Goblin : Race.Elf;
                        yield return Tuple.Create(race, new Point(x,y));
                    }
                }
            }
        }

        public readonly HashSet<Direction> AllDirections
             = Enum.GetValues(typeof(Direction)).Cast<Direction>().ToHashSet();

        public char At(Point point)
            => (point.X < 0 || point.X >= Width)
            ? Wall
            : (point.Y < 0 || point.Y >= Height)
            ? Wall
            : Map[point.X, point.Y];

        public Direction Opposite(Direction direction)
        {
            switch(direction)
            {
                case Direction.Up: return Direction.Down;
                case Direction.Down: return Direction.Up;
                case Direction.Left: return Direction.Right;
                case Direction.Right: return Direction.Left;
            }
            throw new Exception("unprocessed direction - never here");
        }

        public IEnumerable<Direction> Directions(Point start)
        {
            // in which directions we may go from here?
            return AllDirections.Where( d => At(start.Go(d)) == Path );
        }

        public void Move( Man man, Direction direction)
        {
            var newPosition = man.Position.Go(direction);
            Map[man.Position.X, man.Position.Y] = Path;
            man.Position = newPosition;
            Map[man.Position.X, man.Position.Y] = man.Race == Race.Elf ? Elf : Goblin;
        }
    }

    public class Combat {
        readonly List<Man> _men;
        readonly MapGuide _map;

        public Combat( MapGuide map)
        {
            _map = map;
            _men = map.FindAllMen()
                .Select(t=> new Man {HitPoints=200, Position=t.Item2, Race = t.Item1 } )
                .ToList();
        }

        public IEnumerable<Man> MenOfRace(Race race) => _men.Where(m => m.Race == race && m.IsAlive);
        public Race Enemy(Race race) => race == Race.Elf ? Race.Goblin : Race.Elf;
        public bool IsOver => !MenOfRace(Race.Elf).Any() || !MenOfRace(Race.Goblin).Any();

        public void A(Man man)
        {
            var enemies = MenOfRace(Enemy(man.Race));
            // simplest strategy: select path with minimal distance
            enemies.SelectMany(e => e.Position)
        }
    }

    public class Day15
    {
        [TestCase("Day15Input.txt")]
        public void Test1(string file) {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file)).ToArray();
            var combat = new Combat( new MapGuide( lines ) );
        }
    }
}
