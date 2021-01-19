using System;
using System.Collections.Generic;
using System.Linq;

namespace AdventOfCode2018.Day22
{
    public abstract class ToolUtil
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

        static readonly Tool[][] ToolsIn = { ToolsInRocky, ToolsInWet, ToolsInNarow, ToolsInSolidRock };

        public static Tool[] UsableTools(Material material) => ToolsIn[(int)material];

        // access by material (so 4 elements)
        private const int Neither = 1;
        private const int Torch = 2;
        private const int ClimbingGear = 4;

        static readonly int[] FastUsableTools = { ClimbingGear | Torch, ClimbingGear | Neither, Torch | Neither, 0 }; // no tool is usable

        // public enum Tool { Neither, Torch, ClimbingGear
        private static readonly int[] ToolToCode = { Neither, Torch, ClimbingGear };

        public static bool Go(Material a, Material b, Tool tool)
            => a == b || (FastUsableTools[(int)a] & FastUsableTools[(int)b] & ToolToCode[(int)tool]) != 0;

        public static Tool[] ToolsAll = Enum.GetValues(typeof(Tool)).Cast<Tool>().ToArray();

        public static IEnumerable<Tool> OtherTools(Tool tool) => ToolsAll.Where(t => t != tool);

    }
}
