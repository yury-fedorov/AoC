using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace AdventOfCode2018.Day15
{
	public record Point 
	{
        public Point(int x, int y) => (X, Y) = (x, y);
		public int X { get; }
		public int Y { get; }

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
		public string Id; // contains the initial position
	}

	public class Optimizer {
		readonly MapGuide _map;

		readonly Dictionary<Point, int> _cache = new Dictionary<Point, int>();
		readonly Point _p0;
		int _lastCompletedLevel;

		public Optimizer(MapGuide map, Point p0)
		{
			_map = map;
			_p0 = p0;
			_cache.Add(p0, 0);
			_lastCompletedLevel = 0;
		}

		private void Add(Point p, int d)
		{
			int distance;
			if (_cache.TryGetValue(p, out distance))
				Assert.AreEqual(d, distance); 
            else
				_cache.Add(p, d);
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
				Add(p1, distance);
				return distance;
			}
			for (var curDistance = _lastCompletedLevel + 1; !_cache.ContainsKey(p1); curDistance++, _lastCompletedLevel++ )
			{
				// it is completed
				var previousLevel = _cache.Where(p => p.Value == curDistance - 1).Select(p=>p.Key).ToHashSet();
				var nextLevel = previousLevel.SelectMany(p => _map.Directions(p).Select(d=> p.Go(d) ) )
					.Distinct()
					.Where( p => !_cache.ContainsKey(p) )
					.ToHashSet();

				if (!nextLevel.Any())
				{
					// no way to go further
					return null;
				}

				foreach ( var p in nextLevel )
				{
					Add(p, curDistance);
				}
			}
			return _cache[p1];
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

        public IEnumerable<Direction> Directions(Point start) =>
            // in which directions we may go from here?
            AllDirections.Where(d => At(start.Go(d)) == Path);

        public void Move(Man man, Direction direction)
        {
			Assert.True(man.IsAlive, "only alive can move");
            var newPosition = man.Position.Go(direction);
			if ( Path != At(newPosition) )
			{
				Assert.AreEqual(Path, At(newPosition), "we go on the empty space");
			}
			Map[man.Position.X, man.Position.Y] = Path;
            man.Position = newPosition;
            Map[man.Position.X, man.Position.Y] = man.Race == Race.Elf ? Elf : Goblin;
        }

        public bool IsDirectPath(Point p0, Point p1)
        {
			Assert.True(At(p0) != Wall, "p0 == Wall");
			Assert.True(At(p1) != Wall, "p1 == Wall");
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

        // for debug
        public string Draw()
        {
            var picture = new StringBuilder();
            for (int y = 0; y < Map.GetLength(1); y++)
            {
                for (int x = 0; x < Map.GetLength(0); x++)
                {
                    picture.Append(Map[x, y]);
                }
                picture.AppendLine();
            }
            return picture.ToString();
        }
    }

    public class Combat {
        public const int GoblinHitPower = 3;
        readonly List<Man> _men;
        readonly MapGuide _map;
        readonly int _elfHitPower;

        public Combat( MapGuide map, int elfHitPower = GoblinHitPower )
        {
            _elfHitPower = elfHitPower;
            _map = map;
            _men = map.FindAllMen()
                .Select(t=> new Man {HitPoints=200, Position=t.Item2, Race = t.Item1,
					Id = $"{t.Item1}_({t.Item2.X},{t.Item2.Y})" } )
                .ToList();
        }

        public string Draw() => _map.Draw();

		public Man[] Alive => _men.Where(m => m.IsAlive).OrderBy(m => m.Position.ReadingOrder).ToArray();

		public IEnumerable<Man> MenOfRace(Race race) => Alive.Where(m => m.Race == race);
        public Race Enemy(Race race) => race == Race.Elf ? Race.Goblin : Race.Elf;
        public bool IsOver => !MenOfRace(Race.Elf).Any() || !MenOfRace(Race.Goblin).Any();

        // rounds, hitpoints
        public ( int Round, int HitPoints ) Go()
        {
            int round = 0;
            while ( !IsOver ) {
				MakeRound();
                round++;
            }
            return (round, _men.Where(m => m.IsAlive).Sum(m => m.HitPoints));
        }

		public List<Man> MakeRound()
		{
			Assert.False(IsOver, "nothing to do anymore");
			var dead = new List<Man>();
			var alive = Alive;
			foreach (var m in alive) {
				if (!m.IsAlive || IsOver) continue; // could be killed during this loop of battle
				var hitted = ManAction(m);
				if (hitted != null && !hitted.IsAlive)
				{
					dead.Add(hitted);
				}
			}
			return dead;
		}

		public List<Man> MakeSomeRounds(int count)
		{
			var dead = new List<Man>();
			while (count-- > 0 )
			{
				if (IsOver) throw new Exception("game over!");
				dead.AddRange(MakeRound());
			}
			return dead;
		}

        public Man ManAction(Man man)
        {
			Assert.True(_map.At(man.Position) == (man.Race == Race.Elf ? MapGuide.Elf : MapGuide.Goblin), 
				"position on map and within man are not aligned" );

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

            // Task 1: Each unit, either Goblin or Elf, has 3 attack power and starts with 200 hit points.
            // Task 2: elves can have different hit power
            enemyToHit.HitPoints -= man.Race == Race.Elf ? _elfHitPower : GoblinHitPower; 
            if (!enemyToHit.IsAlive)
            {
                // remove the enemy from map and list
                _map.Die(enemyToHit);
                _men.Remove(enemyToHit); // check if it working?
            }
            return enemyToHit;
        }

		public static int DirectionOrder(Direction d)
		{
			switch(d)
			{
				case Direction.Up: return 1;
				case Direction.Left: return 2;
				case Direction.Right: return 3;
				case Direction.Down: return 4;
			}
			throw new Exception("unexpected direction");
		}

		// destination with hint in which direction to go
        public Tuple<Point,Direction> GetDestination(Man man)
        {
            var enemies = MenOfRace(Enemy(man.Race)).ToArray();

            {
                var enimiesDistance = enemies.ToDictionary(e => e, e => e.Position.Distance(man.Position)).ToArray();
                var minDistanceAsIs = enimiesDistance.Min(p => p.Value);
                Assert.True(minDistanceAsIs > 0, "never enemies on the same position");
                if (minDistanceAsIs == 1)
                {
                    // we are in position to attack an enemy, no need to move
                    return null;
                }
            }
			
            // positions in range versus enemies
            var inRange = enemies
                .SelectMany(e => _map.Directions(e.Position).Select(e.Position.Go))
                .ToHashSet()
				.ToDictionary( p => p, p => p.Distance(man.Position) )
				.ToArray()
				; // different enemies can have same in range positions

			// it could happen that all enemies (ie the only one) are rounded from all sides
			// in this way no way to go
			if (!inRange.Any()) return null;

            // simplest strategy: select path with minimal distance
            var minDistance = inRange.Min(p => p.Value);

			if (minDistance == 0) return null; // no way to go

			// more difficult task
			var optimizer = new Optimizer(_map, man.Position);

			var partialResult = new Dictionary<Point, int>();
			int? curMinimalDistance = null;

			foreach ( var inRangePoint in inRange.OrderBy( p => p.Value ) )
			{
				// we try from  by air shortest distance
				if (curMinimalDistance.HasValue)
				{
					if (curMinimalDistance.Value < inRangePoint.Value)
					{
						// the distance already found is less then shortest candidate by air
						// we may terminate
						break;
					}
				}

				var distance = optimizer.Distance(inRangePoint.Key);
				if (distance.HasValue)
				{
					partialResult.Add(inRangePoint.Key, distance.Value);
					curMinimalDistance = curMinimalDistance.HasValue 
						? Math.Min(curMinimalDistance.Value, distance.Value) : distance.Value;
				}
			}

			if (!curMinimalDistance.HasValue)
			{
				// no way to connect two points
				return null;
			}

			var destinationList = partialResult.Where(p => p.Value == curMinimalDistance.Value).Select(p => p.Key);
			var selectedDestination = destinationList.OrderBy(p => p.ReadingOrder).First();

			var optimizer1 = new Optimizer(_map, selectedDestination);
			var options = _map.Directions(man.Position)
				.ToDictionary(d=>d, d => man.Position.Go(d))
				.OrderBy(p=>p.Value.ReadingOrder)
				.ToArray();

			foreach( var o in options)
			{
				var distance = optimizer1.Distance(o.Value); // decreased distance due to the step ahead to the target
				if ( distance.HasValue )
				{
					if ( ( curMinimalDistance.Value - 1 ) == distance.Value )
					{
						// this is the correct direction and expected distance
						return Tuple.Create(selectedDestination, o.Key);
					}
				}
			}
			throw new Exception("unexpected processing of options");
        }
    }

    public class Day15
    {
        // same issue on both tasks (one more round)
        public static int ToAnswer(int round, int hitPoints) => ( round - 1 ) * hitPoints;

        [TestCase("Day15Input.txt", 237490)] // correct answer 1: 85 * 2794 = 237490
        public void Test1(string file, int correctAnswer1 )
        {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file)).ToArray();
            var combat = new Combat(new MapGuide(lines));
            var (round, hitPoints) = combat.Go();
            Assert.AreEqual( correctAnswer1, ToAnswer( round, hitPoints ), "answer 1" );
        }

        [TestCase("Day15Input.txt", 38424)] // correct answer: 24 * 1601 = 38424
        public void Test2(string file, int correctAnswer2 )
        {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file)).ToArray();
            for ( int elfHitPower = Combat.GoblinHitPower; true; elfHitPower++ )
            {
                var combat = new Combat(new MapGuide(lines), elfHitPower);
                var elves = combat.MenOfRace(Race.Elf).Count();
                var (round, hitPoints) = combat.Go();
                if (elves == combat.MenOfRace(Race.Elf).Count())
                {
                    Assert.AreEqual( correctAnswer2, ToAnswer(round, hitPoints), "answer 2" );
                    break;
                }
            }
        }


        [TestCase("Day15Sample1.txt")]
        public void TestSample1(string file)
        {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file)).ToArray();
            var combat = new Combat(new MapGuide(lines));
            var alive = combat.Alive;
            Assert.AreEqual(4, alive.Length, "overall men");
            Assert.AreEqual(1, combat.MenOfRace(Race.Elf).Count(), "overall elfs");
            var firstMan = alive.First();
            Assert.AreEqual(new Point(1, 1), firstMan.Position, "position of first to go");
            var dest = combat.GetDestination(firstMan);
            Assert.AreEqual(new Point(3, 1), dest.Item1, "destination of first man");
            Assert.AreEqual(Direction.Right, dest.Item2, "direction of the first man");
        }

        [TestCase("Day15Sample2.txt")]
        public void TestSample2(string file)
        {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file)).ToArray();
            var combat = new Combat(new MapGuide(lines));

            var elfs = combat.MenOfRace(Race.Elf);
            Assert.AreEqual(1, elfs.Count(), "the only elf is expected");
            var elf = elfs.Single();

            Assert.IsEmpty(combat.MakeRound(), "no dead after 1st round");
            var a = combat.Alive;
            // after the first round
            var g41 = a.Single(m => m.Id == "Goblin_(4,1)");
            var pg41r1 = g41.Position;
            Assert.AreEqual(new Point(2, 1), a.First().Position, "first man");
            Assert.AreEqual(new Point(6, 1), a[1].Position, "second man");
            var p43 = new Point(4, 3);
            Assert.AreEqual(p43, elf.Position, "elf after round 1");

            Assert.IsEmpty(combat.MakeRound(), "no dead after 2st round");
            a = combat.Alive;
            Assert.AreEqual(pg41r1, g41.Position, "g41 must remain where he was");
            Assert.AreEqual(p43, elf.Position, "elf stays on its place");

            Assert.IsEmpty(combat.MakeRound(), "no dead after 13 rounds");
            a = combat.Alive; // refreshed queue
            Assert.AreEqual(p43, elf.Position, "elf position is still fixed");
            Assert.AreEqual(new Point(3, 2), a[0].Position, "man 1");
            Assert.AreEqual(new Point(4, 2), a[1].Position, "man 2");
            Assert.AreEqual(new Point(5, 2), a[2].Position, "man 3");
        }

        public string Who(Combat c) =>
            string.Join('\n', c.Alive.Select(a => $"[{a.Race} ({a.HitPoints}) {a.Position} {a.Id}]").ToArray());

        [TestCase("Day15Sample3.txt")]
        public void TestSample3(string file)
        {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file)).ToArray();
            var combat = new Combat(new MapGuide(lines));
            var firstElf = combat.MenOfRace(Race.Elf).First();
            combat.MakeRound(); // first round
            Assert.AreEqual(197, firstElf.HitPoints);
            combat.MakeRound(); // second round
            Assert.AreEqual(188, firstElf.HitPoints);
            // the second round is over
            // Assert.IsEmpty( combat.MakeSomeRounds(22) ); ??
            combat.MakeSomeRounds(21); // 1 + 1 + 21 = 23
                                       // after 23 rounds
            Assert.False(firstElf.IsAlive);
            var a = combat.Alive;
            Assert.AreEqual(5, a.Length);
            var lastElf = combat.MenOfRace(Race.Elf).Single();
            Assert.AreEqual(131, lastElf.HitPoints);
            Assert.AreEqual(new Point(5, 4), lastElf.Position);
            var who23 = Who(combat);
            var map23 = combat.Draw();

            combat.MakeRound();
            // after 24 rounds
            var who24 = Who(combat);
            var map24 = combat.Draw();

            combat.MakeRound();
            // after 25 rounds
            var who25 = Who(combat);
            var map25 = combat.Draw();

            combat.MakeSomeRounds(3);
            // after 28 rounds
            var who28 = Who(combat);
            var map28 = combat.Draw();

            combat.MakeSomeRounds(19);
            // after 47 rounds
            var who47 = Who(combat);
            var map47 = combat.Draw();

            Assert.True(combat.IsOver);
            Assert.AreEqual(4, combat.Alive.Count());
        }

        [TestCase("Day15Sample3.txt", 47, 590)]
        // [TestCase("Day15Sample4.txt", 37, 982)] -- there is a bug in calculating the number of rounds, in this case it adds one round more
        public void TestSample3Go(string file, int rounds, int hitPoints) {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file)).ToArray();
            var combat = new Combat(new MapGuide(lines));
            var result = combat.Go();
            var who = Who(combat);
            var map = combat.Draw();
            Assert.AreEqual(rounds, result.Item1);
            Assert.AreEqual(hitPoints, result.Item2);
        }
    }
}
