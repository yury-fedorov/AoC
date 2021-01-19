using System;
using System.Collections.Generic;
using System.Linq;

namespace AdventOfCode2018.Day22
{
    public class CostMap
    {
        public record Phase(int X, int Y, Tool Tool) { }
        // how much it costs to arrive here
        readonly IDictionary<Phase, int> _at = new Dictionary<Phase, int>();

        public int? At(int x, int y, Tool tool) => At( new Phase(x, y, tool) );

        public int? At(Phase p) => _at.TryGetValue(p, out var cost) ? cost : null;

        public void Set(int x, int y, Tool tool, int cost)
            => Set( new Phase(x, y, tool), cost );
        public void Set(Phase p, int cost) => _at[p] = cost;

        public bool Optimize( Phase p, int subOptimalCost )
        {
            var exist = _at.TryGetValue(p, out var curCost);
            var newCost = exist ? Math.Min(curCost, subOptimalCost) : subOptimalCost;
            Set(p, newCost);
            return !exist || newCost < curCost; // optimized?
        }

        public readonly (int X, int Y)[] Moves = { (-1, 0), (1, 0), (0, -1), (0, 1) };

        public int Path( Phase from, Phase to, IMap map, int maxTotalCost )
        {
            Set(from, 0);
            var next = new HashSet<Phase>();
            next.Add(from);
            while ( next.Any() )
            {
                var next1 = new HashSet<Phase>();
                foreach( var p0 in next )
                {
                    var p1List = Moves.Select(s => new Point(p0.X + s.X, p0.Y + s.Y))
                        .Where(p => map.At(p) != Material.SolidRock).ToList();
                    foreach (var p1 in p1List)
                    {
                        var ut = ToolUtil.UsableTools(map.At(p1));
                        foreach (var t in ut)
                        {
                            var cost = GetCost(p0.Tool, t);
                            var totalCost = cost + At(p0).Value;
                            if ( totalCost <= maxTotalCost)
                            {
                                var phase1 = new Phase(p1.X, p1.Y, t);
                                if ( Optimize( phase1, totalCost) )
                                {
                                    if (phase1 == to) Console.WriteLine( totalCost );
                                    next1.Add(phase1);
                                }
                            }
                        }

                    }
                }
                next = next1;
            }
            return At(to).Value;
        }

        public static int GetCost(Tool fromTool, Tool toTool)
            => fromTool == toTool ? 1 : 7;
    }
}
