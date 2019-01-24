using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AdventOfCode2018
{
    // solid rock cannot be traversed.
    public enum Material { Rocky, Wet, Narrow, SolidRock }

    public enum Tool { Neither, Torch, ClimbingGear }

    public interface IMap
    {
        Material At(int x, int y);
    }

    public class Map : IMap
    {
        readonly int _depth;
        readonly (int, int) _target;
        readonly IDictionary<(int, int), long> _pointIndexMap = new Dictionary<(int, int), long>();
        public Map(int depth, (int, int) target)
        {
            _depth = depth;
            _target = target;
        }

        public int ToErosionLevel(long geologicalIndex) => (int)((geologicalIndex + _depth) % 20183);

        public long GeologicalIndex((int, int) point)
        {
            long value;
            if (_pointIndexMap.TryGetValue(point, out value)) return value;

            var (x, y) = point;
            if (point == (0, 0)) value = 0;
            else if (point == _target) value = 0;
            else if (y == 0) value = (x * 16807);
            else if (x == 0) value = (y * 48271);
            else value = ToErosionLevel(GeologicalIndex((x - 1, y))) * ToErosionLevel(GeologicalIndex((x, y - 1)));
            _pointIndexMap.Add(point, value);
            return value;
        }

        public Material ToMaterial(long geologicalIndex) => (Material)(ToErosionLevel(geologicalIndex) % 3);

        public Material At(int x, int y) => x >= 0 && y >= 0 ? ToMaterial(GeologicalIndex((x, y))) : Material.SolidRock;

        public string Draw()
        {
            // Rocky, Wet, Narrow - Drawing this same cave system with rocky as ., wet as =, narrow as |
            char[] mapping = { '.', '=', '|' };
            var (tx, ty) = _target;
            var map = new StringBuilder();
            for (var y = 0; y <= ty; y++)
            {
                for (var x = 0; x <= tx; x++)
                {
                    var material = At(x, y);
                    char ch = mapping[(int)material];
                    map.Append(ch);
                }
                map.AppendLine();
            }
            return map.ToString();
        }
    }

    // all 4 sides are bounded
    public class BoundedMap : IMap
    {
        readonly IMap _original; // easily can adopt to calculate also out of cached bounds
        readonly Material[][] _cache;
        readonly int _sizeX;
        readonly int _sizeY;
        public BoundedMap(IMap map, int sizeX, int sizeY)
        {
            _original = map;
            _sizeX = sizeX;
            _sizeY = sizeY;
            _cache = new Material[sizeX][];
            for (int x = 0; x < sizeX; x++)
            {
                _cache[x] = new Material[sizeY];
                for (int y = 0; y < sizeY; y++)
                {
                    _cache[x][y] = _original.At(x, y);
                }
            }
        }
        public Material At(int x, int y)
            => x >= 0 && x < _sizeX && y >= 0 && y < _sizeY
             ? _cache[x][y] : Material.SolidRock;

    }

    // TODO rewrite as a 2D of 1D array
    public class AirMap : Dictionary<(int, int, Tool), int> { }

    public class Optimizer
    {
        readonly IMap _map;
        readonly AirMap _bounds;
        readonly int _longest;
        readonly int _x1;
        readonly int _y1;
        readonly Tool _tool;
        readonly int _switchCount;

        // readonly IDictionary<(int, int), int> _cacheTorch = new Dictionary<(int, int), int>();

        public Optimizer(IMap map, int x1, int y1, Tool tool, AirMap bounds, int switchCount)
        {
            _map = map;
            _x1 = x1;
            _y1 = y1;
            _tool = tool;
            _bounds = bounds;
            _longest = bounds[(x1, y1, tool)];
            _switchCount = switchCount;
        }

        public readonly (int, int) [] shifts = { (-1, 0), (1, 0), (0, -1), (0, 1) };

        const int LongerPath = int.MaxValue >> 1;

        public int Minimize(int x, int y, Tool tool, int path, int switchCount)
        {
            if ( path >= _longest || switchCount >= _switchCount ) return LongerPath;

            int boundedCost;
            if (_bounds.TryGetValue((x, y, tool), out boundedCost))
            {
                // suboptimization says how many steps it is necessary to arrive here
                if (path > boundedCost)
                {
                    return LongerPath; // we exit this path immediately, it is not to go
                }
                if (path < boundedCost )
                {
                    // this is extreamily good, we have just found a shorter path to the point
                    // we need to update the bounds immediately
                    _bounds[(x, y, tool)] = path;
                }
            }

            if (x == _x1 && y == _y1)
            {
                // we are already on the region
                if (tool == _tool) return 0; // target tool is already in hand
                return Day22.OtherTool; // we need only to switch the tool to torch
            }

            var ways = new Dictionary<(int, int, Tool), int>();
            // we still optimizing, going deeper
            // in 4 directions (bounds of negative)
            // switch of tool (in every direction, we can: have 2 tools)
            foreach (var (dx, dy) in shifts)
            {
                var x1 = x + dx;
                var y1 = y + dy;
                var material = _map.At(x1, y1);
                var tools = ToolUtil.Go(material);
                foreach (var t in tools)
                {
                    var isSwitch = tool != t;
                    var dc = Day22.SameTool + (isSwitch ? Day22.OtherTool : 0);
                    var minimized = dc + Minimize(x1, y1, t, path + dc, switchCount + (isSwitch ? 1 : 0) );
                    if ( minimized < LongerPath )
                    {
                        Assert.True(minimized <= _longest);
                        ways.Add((x1, y1, t), minimized);
                    }
                }
            }
            return ways.Any() ? ways.Values.Min() : LongerPath;
        }
    }

    public class ToolUtil
    {
        static readonly Tool[] ToolsInSolidRock = { };

        // In narrow regions, you can use the torch or neither tool. 
        // You cannot use the climbing gear (it's too bulky to fit).
        static readonly Tool[] ToolsInNarow = { Tool.Torch, Tool.Neither };

        // In rocky regions, you can use the climbing gear or the torch.
        // You cannot use neither(you'll likely slip and fall).
        static readonly Tool[] ToolsInRocky = { Tool.ClimbingGear, Tool.Torch };

        // In wet regions, you can use the climbing gear or neither tool.
        // You cannot use the torch (if it gets wet, you won't have a light source).
        static readonly Tool[] ToolsInWet = { Tool.ClimbingGear, Tool.Neither };

        // public enum Material { Rocky, Wet, Narrow, SolidRock }
        static readonly Tool[][] ToolsIn = { ToolsInRocky, ToolsInWet, ToolsInNarow, ToolsInSolidRock };

        public static Tool[] Go(Material material) => ToolsIn[(int)material];

        public static Tool[] ToolsAll = Enum.GetValues(typeof(Tool)).Cast<Tool>().ToArray();

        public static IEnumerable<Tool> OtherTools(Tool tool) => ToolsAll.Where(t => t != tool);

    }

    public class Day22
    {
        public const int SameTool = 1;
        public const int OtherTool = 7;

        // get the wishing tool, if absent, get 2 other tools and select minimal of them + other tool
        public int CostTill(AirMap map, int x, int y, Tool tool)
        {
            if (x < 0 || y < 0) return int.MaxValue;
            int cost;
            if (!map.TryGetValue((x, y, tool), out cost))
            {
                // this tool is no go on the region
                var others = ToolUtil.OtherTools(tool).Where(t => map.ContainsKey((x, y, t)));
                cost = others.Select(t => map[(x, y, t)]).Min() + OtherTool;
            }
            return cost;
        }

        public int MinimalCost(AirMap map, int x, int y, Tool tool)
            => Math.Min(CostTill(map, x - 1, y, tool), CostTill(map, x, y - 1, tool)) + SameTool;

        // Puzzle input
        // depth: 7863
        // target: 14,760
        // [TestCase(510,10,10,114)]
        [TestCase(7863, 14, 760, 11462)] // 786 - too low
        public void Test(int depth, int tx, int ty, int eRiskLevel)
        {
            var map = new Map(depth, (tx, ty));
            int riskLevel = 0;
            for (var x = 0; x <= tx; x++)
            {
                for (var y = 0; y <= ty; y++)
                {
                    var material = map.At(x, y);
                    riskLevel += (int)material;
                }
            }
            // var image = map.Draw();
            Assert.AreEqual(eRiskLevel, riskLevel); // task 1

            var mapAirPath = new AirMap();
            // You start at 0,0 (the mouth of the cave) with the torch equipped
            mapAirPath.Add((0, 0, Tool.Torch), 0);

            Assert.AreEqual(Material.Rocky, map.At(tx, ty));
          
            var maxAirPath = tx + ty;
            for (var diagonal = 1; diagonal <= maxAirPath; diagonal++)
            {
                for (var x = 0; x <= diagonal; x++)
                {
                    var y = diagonal - x;
                    var material = map.At(x, y);
                    var tools = ToolUtil.Go(material); // this tools can be used in this region
                    foreach (var tool in tools)
                    {
                        var cost = MinimalCost(mapAirPath, x, y, tool);
                        Assert.GreaterOrEqual(cost, x + y);
                        mapAirPath.Add((x, y, tool), cost);
                    }
                }
            }
            // Finally, once you reach the target, you need the torch equipped before you can find him in the dark. 
            // The target is always in a rocky region, so if you arrive there with climbing gear equipped, 
            // you will need to spend seven minutes switching to your torch.
            var airDistance = mapAirPath[(tx, ty, Tool.Torch)];
            var dManhattan = airDistance - maxAirPath;
            var absoluteDelta = 1 + dManhattan >> 1; // divide by 2
            var switchCount = dManhattan / OtherTool; // max number of switches
            var fastMap = new BoundedMap(map, tx + absoluteDelta, ty + absoluteDelta);

            // 1656 - this is the upper bond (simplest algorithm)
            // 774 - this is the lower bond (no change of tool at all)
            var optimizer = new Optimizer(fastMap, tx, ty, Tool.Torch, mapAirPath, switchCount);
            var realMinimum = optimizer.Minimize(0, 0, Tool.Torch, 0, 0);
            Assert.AreEqual(-1, realMinimum);
        }
    }
}
