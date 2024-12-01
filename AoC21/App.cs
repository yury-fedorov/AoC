namespace AoC21;
public class App
{
    public static string Directory
    {
        get
        {
            var cd = System.IO.Directory.GetCurrentDirectory();
            if (cd.Contains("AoC21"))
            {
                // For the container case, we don't find this folder
                while (!cd.EndsWith("AoC21"))
                {
                    var pd = System.IO.Directory.GetParent(cd);
                    if (pd == null) throw new Exception("failed to identify the directory");
                    cd = pd.FullName;
                }
            }
            else
            {
                // The container case:
                return "/usr/src/aoc/";
            }
            return cd;
        }
    }

    public static async Task<string[]> ReadLines(string file)
        => await File.ReadAllLinesAsync(Path.Combine(App.Directory, file));

    public static bool IsFast => true;
}

