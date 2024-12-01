using NUnit.Framework;
using NUnit.Framework.Legacy;

namespace AdventOfCode2018.Day22
{
    public enum Material { Rocky, Wet, Narrow, SolidRock } // solid rock cannot be traversed.

    public enum Tool { Neither, Torch, ClimbingGear }

    public record Point(int X, int Y) { };

    public interface IMap {
        Material At(int x, int y);
        Material At(Point p) => At(p.X, p.Y);
    }

    public class Day22
    {
        [TestCase(510,10,10,114,45)]
        // Puzzle input: depth: 7863; target: 14,760
        [TestCase(7863, 14, 760, 11462, 1054)]
        public void Test(int depth, int tx, int ty, int answer1, int answer2)
        {
            var map = new Map(depth, (tx, ty) );
            int riskLevel = 0;
            for (var x = 0; x <= tx; x++) {
                for (var y = 0; y <= ty; y++) {
                    var material = map.At(x, y);
                    riskLevel += (int)material;
                }
            }
            ClassicAssert.AreEqual(answer1, riskLevel, "answer 1");

            if (answer2 > 100 && App.IsFast) return; // takes 21 minutes

            var from = new CostMap.Phase( 0, 0, Tool.Torch );
            var to = new CostMap.Phase( tx, ty, Tool.Torch );

            var MAX = 3*(tx+ty); //this is the upper bond (simplest algorithm)
            var shortestPath = new CostMap().Path( from, to, map, MAX );
            ClassicAssert.AreEqual(answer2, shortestPath, "answer 2");
        }
    }
}
