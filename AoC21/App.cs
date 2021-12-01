namespace AoC21;
public class App {
    public static string Directory {
        get {
            var cd = System.IO.Directory.GetCurrentDirectory();
            while (!cd.EndsWith("AoC21") ) {
                var pd = System.IO.Directory.GetParent(cd);
                cd = pd.FullName;
            }
            return cd;
        }
    }

    public static async Task<string[]> ReadLines(string file) 
        => await File.ReadAllLinesAsync(Path.Combine(App.Directory,file));

    public static bool IsFast => true;
}

