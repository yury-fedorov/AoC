using NUnit.Framework;
using NUnit.Framework.Legacy;

namespace AdventOfCode2018.Day11
{
    public class Field { 
        readonly int [,] map = new int[300, 300];
        public Field(int input) {
            for (int x = 1; x <= 300; x++)
            {
                for (int y = 1; y <= 300; y++)
                {
                    map[x - 1, y - 1] = Day11.PowerAt(x, y, input);
                }
            }
        }

        public (int, int, int) Max(int side)
        {
            int rValue = -23434324;
            int rx = -1;
            int ry = -1;

            for (int x = 1; x <= 300 - side; x++)
            {
                for (int y = 1; y <= 300 - side; y++)
                {
                    int sum = 0;
                    for (int dx = 0; dx < side; dx++)
                    {
                        for (int dy = 0; dy < side; dy++)
                        {
                            sum += map[x - 1 + dx, y - 1 + dy];
                        }
                    }
                    if (rValue < sum)
                    {
                        rx = x;
                        ry = y;
                        rValue = sum;
                    }
                }
            }
            return (rx, ry, rValue);
        }
    }

    public class Day11
    {
        public static int PowerAt(int x, int y, int input) {
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
            int e = ( d / 100 ) % 10;
            var f = e - 5;
            return f;
        }

        const int Serial = 7347;

        [TestCase(3, 5,8,4)]
        // Fuel cell at  122,79, grid serial number 57: power level -5.
        [TestCase(122, 79, 57, -5)]
        // Fuel cell at 217,196, grid serial number 39: power level  0.
        [TestCase(217, 196, 39, 0)]
        // Fuel cell at 101,153, grid serial number 71: power level  4.
        [TestCase(101, 153, 71, 4)]
        public void Test0(int x, int y, int input, int expectedPower)
            => ClassicAssert.AreEqual(expectedPower, PowerAt(x, y, input));

        // For grid serial number 18, the largest total 3x3 square has a top-left corner of 33,45 (with a total power of 29);
        [TestCase(3, 18, 33, 45)]
        // For grid serial number 42, the largest 3x3 square's top-left is 21,61 (with a total power of 30);
        [TestCase(3, 42, 21, 61)]
        [TestCase(3, Serial, 243, 17)] // answer 1
        public void Day11Part1(int side, int input, int ex, int ey)
            => ClassicAssert.AreEqual((ex, ey), Test1(side, input));

        public (int,int) Test1(int side, int input) {
            var map = new int[300,300];
            for ( int x = 1; x <= 300; x++ ) {
                for (int y = 1; y <= 300; y++)
                {
                    map[x - 1, y - 1] = PowerAt(x, y, input);
                }
            }

            int rValue = -23434324;
            int rx = -1;
            int ry = -1;

            for (int x = 1; x <= 300 - side; x++)
            {
                for (int y = 1; y <= 300 - side; y++)
                {
                    int sum = 0;
                    for (int dx = 0; dx < side; dx++)
                    {
                        for (int dy = 0; dy < side; dy++)
                        {
                            sum += map[x - 1 + dx, y - 1 + dy];
                        }
                    }
                    if ( rValue < sum ) {
                        rx = x;
                        ry = y;
                        rValue = sum;
                    }
                }
            }
            return (rx, ry);
        }

        [Ignore("takes too much")]
        // For grid serial number 18, the largest total square (with a total power of 113) is 16x16 
        // and has a top-left corner of 90,269, so its identifier is 90,269,16.
        // For grid serial number 42, the largest total square(with a total power of 119) is 12x12 
        // and has a top-left corner of 232,251, so its identifier is 232,251,12.
        [TestCase(18, 90, 269, 16)]
        [TestCase(42, 232, 251, 12)]
        [TestCase(Serial, 233, 228, 12)] // answer 2
        public void Day11Part2( int input, int ex, int ey, int side ) {
            var r = (-1, -1, -1);
            var os = -1;
            var f = new Field(input);
            for ( int s = 1; s <= 300; s++ ) {
                var por = f.Max(s);
                if (por.Item3 > r.Item3) {
                    r = por;
                    os = s;
                }
            }
            ClassicAssert.AreEqual( (ex,ey,side), (r.Item1,r.Item2,os) );
        }
    }
}
