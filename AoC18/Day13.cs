using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using NUnit.Framework;


namespace AdventOfCode2018.Day13
{
    public class Point : Tuple<int,int> { 
        public Point(int x, int y) : base(x,y) { }
        public int X => Item1;
        public int Y => Item2;
    }

    public enum Direction { Left, Up, Right, Down } // must be in circle for rotation design

    /*
     * Each time a cart has the option to turn (by arriving at any intersection),
     * it turns left the first time,
     * goes straight the second time,
     * turns right the third time,
     * and then repeats those directions starting again with left the fourth time, straight the fifth time, and so on.
     * This process is independent of the particular intersection at which the cart has arrived - that is, the cart has no per-intersection memory.
     */
    public enum Turn { Left, GoStraight, Right }

    public class Car {

        public Point point;
        public Direction direction;
        public Turn nextTurn;
        public bool isRunning;
    }

    public class Day13
    {
        public const string carDirections = "<^>v"; // order is important

        public Direction CharToDirection(char ch) => (Direction)carDirections.IndexOf(ch);

        public int ToShift(Turn turn)
        {
            switch (turn)
            {
                case Turn.Left: return -1;
                case Turn.GoStraight: return 0;
                case Turn.Right: return 1;
                default: Assert.Fail("never");
                    return int.MaxValue;
            }
        }

        public Direction NewDirection(Direction currentDirection, Turn turn)
        {
            int newDirection = ((int)currentDirection + ToShift(turn)) % 4;
            return (Direction) (newDirection >= 0 ? newDirection : (4+newDirection) );
        }

        public Point NewPoint(char[,] road, Point current, Direction direction)
        {
            int dx = 0;
            int dy = 0;
            switch (direction)
            {
                case Direction.Down: dy = 1;
                    break;
                case Direction.Up: dy = -1;
                    break;
                case Direction.Left: dx = -1;
                    break;
                case Direction.Right: dx = 1;
                    break;
            }
            var newPoint = new Point( current.X + dx, current.Y + dy );
            if ( newPoint.X >= road.GetLength(0) || newPoint.X < 0 )
                Assert.Fail( "X is out of bounds" );
            if (newPoint.Y >= road.GetLength(1) || newPoint.Y < 0)
                Assert.Fail("Y is out of bounds");
            return newPoint;
        }

        public Point RunCars( string [] roadMap, bool TillFirstCollision ) {
            // get cars
            var cars = new List<Car>();
            var sizeX = roadMap.First().Length;
            var sizeY = roadMap.Length;

            var road = new char[sizeX, sizeY];
            for (var x = 0; x < sizeX; x++ ) for (var y = 0; y < sizeY; y++) road[x,y] = ' ';

            for (var y = 0; y < roadMap.Length; y++)
            {
                var line = roadMap[y].ToCharArray();
                for (var x = 0; x < line.Length; x++)
                {
                    var ch = line[x];
                    if (carDirections.Contains(ch))
                    {
                        cars.Add( new Car{ point = new Point(x,y), direction = CharToDirection(ch), nextTurn = Turn.Left, isRunning = true } );
                        road[x, y] = "<>".Contains(ch) ? '-' : '|';
                    }
                    else
                    {
                        road[x, y] = ch;
                    }
                }
            }

            bool isLastCarAloneTick = false;
            // once the cars are detected, let us move them
            for (int t = 0; true; t++)
            {
                // Carts all move at the same speed; they take turns moving a single step at a time.
                // They do this based on their current location: carts on the top row move first (acting from left to right),
                // then carts on the second row move (again from left to right), then carts on the third row, and so on.
                // Once each cart has moved one step, the process repeats; each of these loops is called a tick.
                var carTurnQueue = cars.Where(c => c.isRunning).OrderBy(c => ( c.point.Y * 10000 ) + c.point.X ).ToArray();

                foreach (var car in carTurnQueue) // tick loop
                {
                    if ( !car.isRunning ) continue;
                    var curPoint = car.point;
                    var roadCh = road[curPoint.X, curPoint.Y];
                    switch (roadCh)
                    {
                        case '+':
                            // we are on a turn
                            var newDirection = NewDirection(car.direction, car.nextTurn);
                            var nextTurn = (Turn)(((int)car.nextTurn + 1) % 3);
                            car.point = NewPoint(road, car.point, newDirection);
                            car.direction = newDirection;
                            car.nextTurn = nextTurn;
                            break;
                        case '-':
                        case '|':
                            car.point = NewPoint(road, car.point, car.direction);
                            break;
                        case '/':
                            switch (car.direction)
                            {
                                case Direction.Up:
                                    car.direction = Direction.Right;
                                    break;
                                case Direction.Right:
                                    car.direction = Direction.Up;
                                    break;
                                case Direction.Down:
                                    car.direction = Direction.Left;
                                    break;
                                case Direction.Left:
                                    car.direction = Direction.Down;
                                    break;
                            }
                            car.point = NewPoint(road, car.point, car.direction);
                            break;
                        case '\\':
                            switch (car.direction)
                            {
                                case Direction.Up:
                                    car.direction = Direction.Left;
                                    break;
                                case Direction.Right:
                                    car.direction = Direction.Down;
                                    break;
                                case Direction.Down:
                                    car.direction = Direction.Right;
                                    break;
                                case Direction.Left:
                                    car.direction = Direction.Up;
                                    break;
                            }
                            car.point = NewPoint(road, car.point, car.direction);
                            break;
                        default:
                            Assert.Fail("not a part of the road");
                            break;
                    }

                    // post every car move    
                    var grouped = cars.Where(c => c.isRunning).Select(c => c.point).GroupBy(c => c).Where(g => g.Count() > 1);
                    foreach (var group in grouped)
                    {
                        var collisionPoint = group.Key;
                        if (TillFirstCollision) return collisionPoint;
                        // it is not the first one
                        foreach (var crashedCar in cars.Where(c => c.point.Equals(collisionPoint)))
                        {
                            crashedCar.isRunning = false; // mark as crashed
                        }
                    }

                } // end of tick

                // What is the location of the last cart at the end of the first tick where it is the only cart left?
                var runningCars = cars.Where(c => c.isRunning).ToArray();
                if (runningCars.Length == 1)
                {
                    return runningCars.Single().point;
                }
                    
            }  // end of the time circle
        }

        [Ignore("done")]
        [TestCase("Day13Sample1.txt", true, 0,3)]
        [TestCase("Day13Sample2.txt", true, 7,3)]
        [TestCase("Day13Input.txt", true, 76, 108)] // correct 1: "76,108"
        [TestCase("Day13Sample3.txt", false, 6, 4)] // test case
        [TestCase("Day13Input.txt", false, 2,84)] // correct 2: "2,84"
        // second (incorrect): 59, 9
        // second (incorrect): 29,104
        // second (incorrect): 40,20
        public void Test1(string file, bool tillFirstCrash, int ex, int ey) {
            var point = new Point(ex, ey);
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file)).ToArray();
            var collision = RunCars(lines,tillFirstCrash);
            Assert.AreEqual(point, collision);
        }
    }
}
