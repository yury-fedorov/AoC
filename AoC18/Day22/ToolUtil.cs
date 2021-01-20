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
    }
}
