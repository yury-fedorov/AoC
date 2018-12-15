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

		public int ReadingOrder => (Y * 1000) + X;
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

	public class Optimizer {
		readonly MapGuide _map;

		readonly Dictionary<Point, int> _cache = new Dictionary<Point, int>();
		readonly Point _p0;

		public Optimizer(MapGuide map, Point p0)
		{
			_map = map;
			_p0 = p0;
			_cache.Add(p0, 0);
		}

		private void Add(Point p, int d)
		{
			int distance;
			if (_cache.TryGetValue(p, out distance))
			{
				Assert.AreEqual(d, distance); 
			} else
			{
				_cache.Add(p, d);
			}
		}

		public int ? Distance( Point p1 )
		{
			int distance;
			if ( _cache.TryGetValue(p1, out distance) )
			{
				// it is already within cache
				return distance;
			}
			if ( _map.IsDirectPath(_p0,p1))
			{
				// air path
				distance = _p0.Distance(p1);
				Add(p1, _p0.Distance(p1));
				return distance;
			}
			// this is extremely costly for stack
			/*
			var p2 = _map.Directions(p1).Select(d => p1.Go(d));
			var dic = p2.ToDictionary(p => p, p => Distance(p)).Where(a => a.Value.HasValue).ToArray();
			if (dic.Length == 0) return null; // no path
			distance = dic.Min(a => a.Value.Value) + 1;
			_cache.Add(p1, distance);
			return distance;
			*/
			var maxSteps = _map.Height * _map.Width; // absolutely pessimistic
			for (var curDistance = 1; curDistance < maxSteps; curDistance++ )
			{
				var previousLevel = _cache.Where(p => p.Value == curDistance - 1).Select(p=>p.Key).ToHashSet();
				var nextLevel = previousLevel.SelectMany(p => _map.Directions(p).Select(d=> p.Go(d) ) )
					.Where( p => !_cache.ContainsKey(p) ).ToHashSet();

				if (!nextLevel.Any())
				{
					// no way to go further
					return null;
				}

				foreach ( var p in nextLevel )
				{
					if (p == p1) return distance;
					Add(p, curDistance);
				}
			}
			throw new Exception("something wrong");
		}
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

        internal void Die(Man dead)
        {
            Map[dead.Position.X, dead.Position.Y] = Path; // empty space
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

        // rounds, hitpoints
        public Tuple<int, int> Go()
        {
            int round = 0;
            while ( !IsOver)
            {
                var alive = _men.Where(m => m.IsAlive).OrderBy(m => m.Position.ReadingOrder).ToArray();
                foreach( var m in alive)
                {
                    if (!m.IsAlive) continue; // could be killed during this loop of battle
                    var hitted = ManAction(m);
                }
                round++;
            }
            return Tuple.Create(round, _men.Where(m => m.IsAlive).Sum(m => m.HitPoints));
        }

        public Man ManAction(Man man)
        {
            // assumption - do move and hit immediately
            var destinationDirection = GetDestination(man);
            if (destinationDirection != null && destinationDirection.Item1.Distance(man.Position) > 0)
            {
				var direction = destinationDirection.Item2;
                _map.Move(man,direction);
            }

            // get enemy and hit him 
            // with the fewest hit points is selected
            var adjacentEnemies = MenOfRace(Enemy(man.Race)).Where(m => m.Position.Distance(man.Position) == 1)
                .ToArray();

			if (adjacentEnemies.Length == 0) return null; // no enemy to hit

            var minHitPoints = adjacentEnemies.Min(m => m.HitPoints);
            var enemyToHit = adjacentEnemies.Where(m => m.HitPoints == minHitPoints)
                .OrderBy(m => m.Position.ReadingOrder ) // reading order
                .First();

            // Each unit, either Goblin or Elf, has 3 attack power and starts with 200 hit points.
            enemyToHit.HitPoints -= 3; 
            if (!enemyToHit.IsAlive)
            {
                // remove the enemy from map and list
                _map.Die(enemyToHit);
                _men.Remove(enemyToHit); // check if it working?
            }
            return enemyToHit;
        }

        public Direction ? WhereToMove(Point position, Point destination)
        {
            var dx = destination.X - position.X;
            if ( dx != 0)
            {
                var xd = dx > 0 ? Direction.Right : Direction.Left;
                var xp = position.Go( xd );
                if ( _map.IsDirectPath(xp,destination))
                {
                    return xd;
                }
            }
            var dy = destination.Y - position.Y;
            if (dy != 0)
            {
                var yd = dy > 0 ? Direction.Down : Direction.Up;
                var yp = position.Go(yd);
                if (_map.IsDirectPath(yp, destination))
                {
                    return yd;
                }
            }
            // seems the path is more complicated
            throw new Exception("more complicated path");
        }

		// destination with hint in which direction to go
        public Tuple<Point,Direction> GetDestination(Man man)
        {
            var enemies = MenOfRace(Enemy(man.Race));

            // positions in range versus enemies
            var inRange = enemies
                .SelectMany(e => _map.Directions(e.Position).Select(e.Position.Go))
                .ToHashSet(); // different enemies can have same in range positions

            // simplest strategy: select path with minimal distance
            var minDistance = inRange.Select( man.Position.Distance ).Min();

			if (minDistance == 0) return null; // no way to go

            // not sure they are shortest indeed to be prooved
            var nearestByAir = inRange.Where(p => man.Position.Distance(p) == minDistance);

            var nearestByAirIndeed = 
                nearestByAir.Where(p => _map.IsDirectPath(p, man.Position)).ToArray();

            if (nearestByAirIndeed.Any())
            {
                // we have found the destination
				var destination = nearestByAirIndeed.OrderBy(p => p.ReadingOrder).First();
				return Tuple.Create(destination, WhereToMove(man.Position, destination).Value);
            }
            else
            {
				// more difficult task
				var optimizer = new Optimizer(_map, man.Position);
				var pointDistance = 
					inRange.ToDictionary(p => p, p => optimizer.Distance(p))
						.Where(p => p.Value.HasValue)
						.ToDictionary(p => p.Key, p => p.Value.Value);

				if (!pointDistance.Any()) return null; // no way to go

				var minimalDistance = pointDistance.Values.Min();

				var destinations = pointDistance.Where(p => p.Value == minimalDistance).Select(p => p.Key)
					.ToArray();

				var options = _map.Directions(man.Position).ToArray();

				var directionDestination = new List<Tuple<Direction,Point>>();
				foreach (var o in options)
				{
					var p1 = man.Position.Go(o);
					var optimizer1 = new Optimizer(_map, p1);
					foreach (var d in destinations)
					{
						var dd = optimizer1.Distance(d);
						if (dd.HasValue)
						{
							if (dd.Value < minimalDistance) // we expect here minimal - 1
							{
								// this is correct direction
								directionDestination.Add(Tuple.Create(o,d));
							}
						}
					}
				}
				// now the direction destination is ready
				if (!directionDestination.Any()) return null;

				var first = directionDestination.OrderBy(dd => dd.Item2.ReadingOrder).First();
				return Tuple.Create(first.Item2, first.Item1); // todo - just inversion of the tuple
			}
        }
    }

    public class Day15
    {
        [TestCase("Day15Input.txt")]
        public void Test1(string file) {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file)).ToArray();
            var combat = new Combat( new MapGuide( lines ) );
			var result = combat.Go();
			Assert.AreEqual((0,0), result);
        }
    }
}
