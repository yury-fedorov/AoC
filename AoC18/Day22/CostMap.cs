using System;
using System.Collections.Generic;
using System.Linq;

namespace AdventOfCode2018.Day22
{
    public class CostMap
    {
        public record Phase(int X, int Y, Tool Tool) { }
        public record CostInfo(int Cost, HashSet<Phase> Steps) { }
        // how much it costs to arrive here
        readonly IDictionary<Phase, CostInfo> _at = new Dictionary<Phase, CostInfo>();

        public int? At(int x, int y, Tool tool) => At(new Phase(x, y, tool));

        public int? At(Phase p) => _at.TryGetValue(p, out var cost) ? cost.Cost : null;

        public void Set(int x, int y, Tool tool, CostInfo cost)
            => Set(new Phase(x, y, tool), cost);
        public void Set(Phase p, CostInfo cost) => _at[p] = cost;

        public bool Optimize(Phase p, CostInfo subOptimal)
        {
            var exist = _at.TryGetValue(p, out var curCostInfo);
            var isUpdated = !exist || subOptimal.Cost < curCostInfo.Cost;
            if (isUpdated) _at[p] = subOptimal;
            return isUpdated; // optimized?
        }

        public readonly (int X, int Y)[] Moves = { (1, 0), (0, 1), (0, -1), (-1, 0) };

        // TODO - better quick maxTotalCost detection could significantly reduce processing time
        // ie. moving only for the given number of path == (x + y) distance and only positive increments:
        // so every step only closer to the target.
        public int Path(Phase from, Phase to, IMap map, int maxTotalCost)
        {
            Set(from, new CostInfo(0, new HashSet<Phase>()));
            var moves = Moves; // .Where(s => to.X > to.Y ? (s.X >= 0) : (s.Y >= 0));
            var next = new HashSet<Phase>();
            next.Add(from);
            for (var path = 0; next.Any(); path++)
            {
                var next1 = new HashSet<Phase>();
                foreach (var p0 in next)
                {
                    var ci0 = _at[p0];
                    var p1List = moves.Select(s => new Point(p0.X + s.X, p0.Y + s.Y));
                    foreach (var p1 in p1List)
                    {
                        if (!ToolUtil.UsableTools(map.At(p1)).Contains(p0.Tool))
                            continue;
                        var phase1 = new Phase(p1.X, p1.Y, p0.Tool);
                        Step(maxTotalCost, next1, ci0, phase1, 1);
                    }
                    var otherTool = ToolUtil.UsableTools(map.At(p0.X, p0.Y))
                        .Single(t => t != p0.Tool);
                    var phase10 = new Phase(p0.X, p0.Y, otherTool);
                    Step(maxTotalCost, next1, ci0, phase10, 7);
                }
                var solution = At(to);
                if (solution.HasValue)
                {
                    if (solution.Value < path)
                        return solution.Value; // no sense to go further
                    maxTotalCost = Math.Min(maxTotalCost, solution.Value);
                }
                next = next1;
            }
            return At(to).Value;
        }

        void Step(int maxTotalCost, ICollection<Phase> next1, CostInfo ci0, Phase phase, int cost)
        {
            if (ci0.Steps.Contains(phase)) return; // we were already here
            var totalCost = cost + ci0.Cost;
            if (totalCost <= maxTotalCost)
            {
                var dp1 = new HashSet<Phase>(ci0.Steps);
                dp1.Add(phase);
                if (Optimize(phase, new CostInfo(totalCost, dp1)))
                    next1.Add(phase);
            }
        }
    }
}
