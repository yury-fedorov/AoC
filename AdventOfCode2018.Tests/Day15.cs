using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using NUnit.Framework;

namespace AdventOfCode2018.Day15
{
    public class Point : Tuple<int, int>
    {
        public Point(int x, int y) : base(x, y) { }
        public int X => Item1;
        public int Y => Item2;

        public int Distance(Point other) => Math.Abs(X - other.X) + Math.Abs(Y - other.Y);

        public Point Go(Direction direction) {
            var dx = direction == Direction.Left ? -1 : direction == Direction.Right ? 1 : 0;
            var dy = direction == Direction.Up ? -1 : direction == Direction.Down ? 1 : 0;
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

        public IEnumerable<Tuple<Race, Point>> FindAllMen()
        {
            for (int x = 0; x < Width; x++)
            {
                for (int y = 0; y < Height; y++)
                {
                    var cell = Map[x, y];
                    if (cell == Goblin || cell == Elf)
                    {
                        var race = cell == Goblin ? Race.Goblin : Race.Elf;
                        yield return Tuple.Create(race, new Point(x, y));
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
            switch (direction)
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
            return AllDirections.Where(d => At(start.Go(d)) == Path);
        }

        public void Move(Man man, Direction direction)
        {
            var newPosition = man.Position.Go(direction);
            Map[man.Position.X, man.Position.Y] = Path;
            man.Position = newPosition;
            Map[man.Position.X, man.Position.Y] = man.Race == Race.Elf ? Elf : Goblin;
        }

        public bool IsDirectPath(Point p0, Point p1)
        {
            var distance = p0.Distance(p1);
            if (distance < 2) return true;
            var dx = p1.X - p0.X;
            var options = new List<Point>();
            if (dx != 0)
            {
                // we move closer by X
                options.Add( new Point(p0.X + Math.Sign(dx),p0.Y));
            }
            var dy = p1.Y - p0.Y;
            if (dy != 0)
            {
                // we move closer by Y
                options.Add(new Point(p0.X, p0.Y + Math.Sign(dy)));
            }
            return options.Where(p => At(p) == Path).Any(p => IsDirectPath(p, p1));
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

        public Point WhereToMove(Man man)
        {
            var destination = GetDestination(man);
            var dx = destination.X - man.Position.X;
            if ( dx != 0)
            {
                var xp = man.Position.Go( dx > 0 ? Direction.Right : Direction.Left );
                if ( _map.IsDirectPath(xp,destination))
                {
                    return xp;
                }
            }
            var dy = destination.Y - man.Position.Y;
            if (dy != 0)
            {
                var yp = man.Position.Go(dy > 0 ? Direction.Down : Direction.Up);
                if (_map.IsDirectPath(yp, destination))
                {
                    return yp;
                }
            }
            // seems the path is more complicated
            throw new Exception("more complicated path");
        }

        public Point GetDestination(Man man)
        {
            var enemies = MenOfRace(Enemy(man.Race));

            // positions in range versus enemies
            var inRange = enemies
                .SelectMany(e => _map.Directions(e.Position).Select(e.Position.Go))
                .ToArray();

            // simplest strategy: select path with minimal distance
            var minDistance = inRange.Select( man.Position.Distance ).Min();

            // not sure they are shortest indeed to be prooved
            var nearestByAir = inRange.Where(p => man.Position.Distance(p) == minDistance);

            var nearestByAirIndeed = 
                nearestByAir.Where(p => _map.IsDirectPath(p, man.Position)).ToArray();

            if (nearestByAirIndeed.Any())
            {
                // check how many
                if ( nearestByAirIndeed.Length == 1)
                {
                    // we have found the destination
                    return nearestByAirIndeed.Single();
                }
                else
                {
                    // we need to choose among equal options
                    throw new Exception("choose among equal");
                }
            }
            else
            {
                // more difficult task
                throw new Exception("longer then by air distance optimization"); 
            }
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
