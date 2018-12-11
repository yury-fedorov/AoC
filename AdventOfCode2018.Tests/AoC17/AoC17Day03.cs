using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Diagnostics;
using System;
using NUnit.Framework;

namespace AdventOfCode2018.Tests.AoC17.Day3
{
    public class Point : Tuple<int,int>
    {
        public int X => Item1;
        public int Y => Item2;
        public int MD => Math.Abs(X) + Math.Abs(Y);
        public int Level => Math.Max( Math.Abs(X), Math.Abs(Y));
        public bool IsEdge => Math.Abs(X) == Math.Abs(Y);
        public Point(int x, int y) : base(x,y)
        {
        }
        public bool LastOnLevel => Level == 0 || (X > 0 && Y == -Level);
        public Point Next()
        {
            if (LastOnLevel) return new Point(Level + 1, -Level);
            if (IsEdge)
            {
                int dx = 0;
                int dy = 0;
                if (X > 0 && Y > 0) dx = -1;
                else if (X < 0 && Y > 0) dy = -1;
                else if (X < 0 && Y < 0) dx = 1;
                else dy = 1;
                return new Point( X + dx, Y + dy );
            }
            var isMovingX = Math.Abs(X) < Level;
            if (isMovingX) return new Point(X - Math.Sign(Y), Y);
            return new Point(X, Y + Math.Sign(X));
        }
    }

    public class AoC17Day03
    {
        public long ValueAt(int index)
        {
            var map = new Dictionary<Point, long>();
            int i = 1;
            var point = new Point(0, 0);
            long value;
            map.Add(point, value = i++);
            for (; i <= index; i++)
            {
                point = point.Next();
                value = map.Where(p => Math.Abs(p.Key.X - point.X) <= 1
                   && Math.Abs(p.Key.Y - point.Y) <= 1).Sum(p => (long)p.Value);
                map.Add(point, value);
            }
            return value;
        }

        [TestCase(1,1)]
        [TestCase(2,1)]
        [TestCase(3,2)]
        [TestCase(4,4)]
        [TestCase(5,5)]
        [TestCase(6,10)]
        [TestCase(7,11)]
        [TestCase(8,23)]
        [TestCase(9,25)]
        [TestCase(10,26)]
        [TestCase(11,54)]
        [TestCase(23, 806)]
        public void TestAt(int index, long expected)
        {
            Assert.AreEqual(expected, ValueAt(index));
        }

        [TestCase(347991)]
        // [TestCase(26)]
        public void Task2(int input )
        {
            for (int i = 1; i < input; i++ )
            {
                var r = ValueAt(i);
                if (r > input) Assert.Fail( $"{i} {r}" );
            }
        }
    }
}