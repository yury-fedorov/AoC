using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;

namespace AdventOfCode2018
{
    public enum Material { Rocky, Wet, Narrow }

    public enum Tool { Neither, Torch, ClimbingGear }

    public class Map
    {
        readonly int _depth;
        readonly (int, int) _target;
        readonly IDictionary<(int, int), BigInteger> _pointIndexMap = new Dictionary<(int, int), BigInteger>();
        public Map(int depth, (int,int) target)
        {
            _depth = depth;
            _target = target;
        }

        public int ToErosionLevel(BigInteger geologicalIndex) => (int)((geologicalIndex + _depth) % 20183);

        public BigInteger GeologicalIndex( (int,int) point )
        {
            BigInteger value;
            if (_pointIndexMap.TryGetValue(point, out value)) return value;

            var (x, y) = point;
            if (point == (0, 0) ) value = 0;
            else if (point == _target) value = 0;
            else if (y == 0) value = (x * 16807);
            else if (x == 0) value = (y * 48271);
            else value = ToErosionLevel(GeologicalIndex( (x-1, y) )) * ToErosionLevel(GeologicalIndex( (x, y-1) ));
            _pointIndexMap.Add(point, value);
            return value;
        }

        public Material ToMaterial(BigInteger geologicalIndex) => (Material)( ToErosionLevel(geologicalIndex) % 3 );

        public Material At((int, int) point) => ToMaterial(GeologicalIndex(point));

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
                    var material = At((x, y));
                    char ch = mapping[(int)material];
                    map.Append(ch);
                }
                map.AppendLine();
            }
            return map.ToString();
        }
    }

    public class AirMap : Dictionary<(int, int, Tool), int> { }

    public class Optimizer
    {
        readonly Map _map;
        readonly AirMap _bounds;
        readonly int _longest;
        readonly int _x1;
        readonly int _y1;
        readonly Tool _tool;

        // readonly IDictionary<(int, int), int> _cacheTorch = new Dictionary<(int, int), int>();

        public Optimizer(Map map, int x1, int y1, Tool tool, AirMap bounds) {
            _map = map;
            _x1 = x1;
            _y1 = y1;
            _tool = tool;
            _bounds = bounds;
            _longest = bounds[(x1,y1,tool)];
        }

        public readonly IEnumerable<(int, int)> shifts = new[] { (-1,0), (1,0), (0,-1), (0,1) };

        public int Minimize( int x, int y, Tool tool, int path )
        {
            if (path >= _longest) return path;

            int boundedCost;
            if ( _bounds.TryGetValue( (x,y,tool), out boundedCost ) )
            {
                // suboptimization says how many steps it is necessary to arrive here
                if ( path > boundedCost )
                {
                    return _longest; // we exit this path immediately, it is not to go
                }
            }

            if ( x == _x1 && y == _y1 )
            {
                // we are already on the region
                if (tool == _tool) return 0; // target tool is already in hand
                return Day22.OtherTool; // we need only to switch the tool to torch
            }

            var ways = new Dictionary<(int,int,Tool),int>();
            // we still optimizing, going deeper
            // in 4 directions (bounds of negative)
            // switch of tool (in every direction, we can: have 2 tools)
            foreach ( var (dx,dy) in shifts )
            {
                var x1 = x + dx;
                var y1 = y + dy;
                if (x1 < 0 || y1 < 0) continue; // negative bounds
                var material = _map.At((x1, y1));
                var tools = Day22.Go(material);
                foreach ( var t in tools )
                {
                    var dc = Day22.SameTool + ( tool == t ? 0 : Day22.OtherTool );
                    var minimized = dc + Minimize(x1, y1, t, path + dc);
                    ways.Add((x1, y1, t), minimized );
                }
            }
            return ways.Values.Min();
        }
    }

    public class Day22
    {
        // Tools can only be used in certain regions:
        public static Tool NoGo(Material material)
        {
            switch(material)
            {
                // In narrow regions, you can use the torch or neither tool. 
                // You cannot use the climbing gear (it's too bulky to fit).
                case Material.Narrow: return Tool.ClimbingGear;

                // In rocky regions, you can use the climbing gear or the torch.
                // You cannot use neither(you'll likely slip and fall).
                case Material.Rocky: return Tool.Neither;

                // In wet regions, you can use the climbing gear or neither tool.
                // You cannot use the torch (if it gets wet, you won't have a light source).
                case Material.Wet: return Tool.Torch;
            }
            throw new Exception("not expected material");
        }
        
        public static IEnumerable<Tool> Go(Material material) => OtherTools(NoGo(material));

        public static IEnumerable<Tool> OtherTools(Tool tool) => Enum.GetValues(typeof(Tool)).Cast<Tool>().Where(t => t != tool);
    
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
                var others = OtherTools(tool).Where(t => map.ContainsKey( (x,y,t) ) );
                cost = others.Select(t => map[(x, y, t)]).Min() + OtherTool;
            }
            return cost;
        }

        public int MinimalCost( AirMap map, int x, int y, Tool tool )
            => Math.Min( CostTill(map,x-1,y,tool), CostTill(map,x,y-1,tool) ) + SameTool;

        // Puzzle input
        // depth: 7863
        // target: 14,760
        // [TestCase(510,10,10,114)]
        [TestCase(7863, 14, 760, 11462)] // 786 - too low
        public void Test(int depth, int tx, int ty, int eRiskLevel)
        {
            var map = new Map(depth, (tx,ty) );
            var a =  map.At((1, 1));
            int riskLevel = 0;
            for (var x = 0; x <= tx; x++ )
            {
                for (var y = 0; y <= ty; y++)
                {
                    var material = map.At((x, y));
                    riskLevel += (int)material;
                }
            }
            var image = map.Draw();
            Assert.AreEqual(eRiskLevel, riskLevel);

            var mapAirPath = new AirMap();
            // You start at 0,0 (the mouth of the cave) with the torch equipped
            mapAirPath.Add((0, 0, Tool.Torch), 0);

            Assert.AreEqual(Material.Rocky, map.At((tx, ty)));

            var maxAirPath = tx + ty;
            for (var diagonal = 1; diagonal <= maxAirPath; diagonal++)
            {
                for (var x = 0; x <= diagonal; x++)
                {
                    var y = diagonal - x;
                    var material = map.At((x, y));
                    var tools = Go(material); // this tools can be used in this region
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

            // 1656 - this is the upper bond (simplest algorithm)
            // 774 - this is the lower bond (no change of tool at all)
            var optimizer = new Optimizer(map, tx, ty, Tool.Torch, mapAirPath);
            var realMinimum = optimizer.Minimize(0, 0, Tool.Torch, 0);
            Assert.AreEqual(-1, realMinimum);
        }
    }
}
