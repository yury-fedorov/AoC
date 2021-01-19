using System;
using NUnit.Framework;

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
        // [TestCase(7863, 14, 760, 11462, -1)] // 786 - too low
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
            Assert.AreEqual(answer1, riskLevel, "answer 1");

            var from = new CostMap.Phase( 0, 0, Tool.Torch );
            var to = new CostMap.Phase( tx, ty, Tool.Torch );

            var shortestPath = new CostMap().Path( from, to, map, 3*(tx + ty) );

            if ( answer2 < 0 )
            {
                var MIN = 1036;  // this is the lower bond (no change of tool at all)
                var MAX = Math.Min(1656, 7 * MIN); //this is the upper bond (simplest algorithm)
                Assert.True( shortestPath > MIN, "too small " + shortestPath);
                Assert.True( shortestPath < MAX, "too long " + shortestPath);
            }

            Assert.AreEqual(answer2, shortestPath, "answer 2");

            /*
            var smallCostMap = new CostMap(tx+1, ty+1);
            // You start at 0,0 (the mouth of the cave) with the torch equipped
            smallCostMap.Set( 0, 0, Tool.Torch, 0 );

            Assert.AreEqual(Material.Rocky, map.At(tx, ty), "rocky target");

            var manhattanPath = tx + ty;
            for (var diagonal = 1; diagonal <= manhattanPath; diagonal++) {
                for (var x = 0; x <= diagonal; x++) {
                    var y = diagonal - x;
                    var material = map.At(x, y);
                    var tools = ToolUtil.UsableTools(material); // this tools can be used in this region
                    foreach (var tool in tools) {
                        var cost = smallCostMap.MinimalCost(x, y, tool);
                        Assert.GreaterOrEqual(cost, x + y, "cost more or equal manhattan" );
                        smallCostMap.Set( x, y, tool, cost);
                    }
                }
            }
            // Finally, once you reach the target, you need the torch equipped before you can find him in the dark. 
            // The target is always in a rocky region, so if you arrive there with climbing gear equipped, 
            // you will need to spend seven minutes switching to your torch.
            var airDistance = smallCostMap.At( tx, ty, Tool.Torch);
            var dManhattan = airDistance - manhattanPath;
            var absoluteDelta = 1 + dManhattan >> 1; // divide by 2
            var switchCount = dManhattan / CostMap.OtherTool; // max number of switches
            var fastMap = new BoundedMap(map, tx + absoluteDelta, ty + absoluteDelta);

            // for the next phase we need a bigger cost map
            var costMap = new CostMap(tx + absoluteDelta, ty + absoluteDelta, smallCostMap);

            // now let us play with partial optimizations
            for (var diagonal = 1; diagonal <= manhattanPath; diagonal++) {
                for (var x = 0; x <= diagonal; x++) {
                    var y = diagonal - x;
                    var material = map.At(x, y);
                    var tools = ToolUtil.UsableTools(material); // this tools can be used in this region
                    foreach (var tool in tools) {
                        var prevCost = costMap.At(x, y, tool);
                        if ( prevCost == CostMap.ND) // we never done any estimation for this point
                        {
                            var oneStepCost = costMap.MinimalCost(x, y, tool);
                            prevCost = oneStepCost;
                        }
                        Assert.GreaterOrEqual(prevCost, x + y, "prev cost vs manhattan");
                        var manhattan1 = x + y;
                        var dManhattan1 = prevCost - manhattan1;
                        int switchCount1 = dManhattan1 / CostMap.OtherTool;
                        var optimizer1 = new Optimizer(fastMap, x , ty, tool, costMap, switchCount1);
                        var bestCost = optimizer1.Minimize(0, 0, Tool.Torch, 0, 0);
                        Assert.GreaterOrEqual(prevCost, bestCost, "prev cost vs best cost" );
                        costMap.Set(x, y, tool, bestCost);
                    }
                }
            }

            // 1656 - this is the upper bond (simplest algorithm)
            // 774 - this is the lower bond (no change of tool at all)
            // var optimizer = new Optimizer(fastMap, tx, ty, Tool.Torch, costMap, switchCount);
            // var realMinimum = optimizer.Minimize(0, 0, Tool.Torch, 0, 0);

            var realMinimum = costMap.At(tx, ty, Tool.Torch);
            Assert.AreEqual(-1, realMinimum, "answer 2");
            */
        }
    }
}
