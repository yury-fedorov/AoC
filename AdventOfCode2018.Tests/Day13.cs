using System;
using System.IO;
using System.Linq;
using NUnit.Framework;

namespace AdventOfCode2018.Day13
{
    public class Point : Tuple<int,int> { 
        public Point(int x, int y) : base(x,y) { }
    }

    public enum Direction { Up, Down, Left, Right }

    public enum Turn { Left, Right, GoAhead }

    public class Car {

        public Point point;
        public Direction direction;
        public Turn nextTurn; 
    }

    public class Day13
    {
        public const string carDirections = "<>v^";

        public Point DetectCollision( string [] roadMap ) {
            return new Point(0, 0);
        }

        [TestCase("Day13Sample1.txt", 0,3)]
        [TestCase("Day13Sample2.txt", 7,3)]
        [TestCase("Day13Input.txt", -1,-1)]
        public void Test1(string file, int ex, int ey) {
            var point = new Point(ex, ey);
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file)).ToArray();
            var collision = DetectCollision(lines);
            Assert.AreEqual(point, collision);
        }
    }
}
