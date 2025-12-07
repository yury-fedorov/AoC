namespace AdventOfCode2018
{
    public class App
    {
        public static string Directory
        {
            get
            {
                var cd = System.IO.Directory.GetCurrentDirectory();
                if (cd.Contains("AoC18"))
                {
                    while (!cd.EndsWith("AoC18"))
                    {
                        var pd = System.IO.Directory.GetParent(cd);
                        cd = pd!.FullName;
                    }
                }
                else
                {
                    cd = "/usr/src/aoc";
                }
                return cd;
            }
        }

        public static bool IsFast => true;
    }
}
