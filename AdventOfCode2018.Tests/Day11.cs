using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Diagnostics;
using System;
using NUnit.Framework;

//  || {} []

namespace AdventOfCode2018.Tests.Day11
{
    public class Day11
    {

        public int PowerAt(int x, int y, int input) {
            /* Find the fuel cell's rack ID, which is its X coordinate plus 10.
Begin with a power level of the rack ID times the Y coordinate.
Increase the power level by the value of the grid serial number (your puzzle input).
Set the power level to itself multiplied by the rack ID.
Keep only the hundreds digit of the power level (so 12345 becomes 3; numbers with no hundreds digit become 0).
Subtract 5 from the power level. */
            var rankId = x + 10;
            var b = rankId * y;
            var c = b + input;
            var d = rankId * c;
            int e = d / 100;
            var f = e - 5;
            return f;
        }



        [TestCase(3, 7347)]
        public void Test1(int side, int input) {
            var map = new Dictionary<Tuple<int, int>, int>();
            for ( int x = 1; x < 300; x++ ) {
                for (int y = 1; y < 300; y++)
                {
                    map.Add( Tuple.Create(x,y), PowerAt(x,y,input) );
                }
            }

            int rValue = -23434324;
            int rx = -1;
            int ry = -1;

            for (int x = 1; x < 300; x++)
            {
                for (int y = 1; y < 300; y++)
                {
                    var sum = map.Where(p =>
                        Math.Abs(p.Key.Item1 - x) <= 1
                     && Math.Abs(p.Key.Item2 - y) <= 1)
                    .Select(p => p.Value).Sum();

                    if ( rValue < sum ) {
                        rx = x;
                        ry = y;
                        rValue = sum;
                    }
                }
            }

            Assert.AreEqual( Tuple.Create(rx,ry), Tuple.Create(-1,-1) );
        }

        //  || {} []
    }
}
