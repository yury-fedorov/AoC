namespace AdventOfCode2017 {
    public class App {
        public static string Directory {
            get {
                var cd = System.IO.Directory.GetCurrentDirectory();
                while (!cd.EndsWith("AoC17") ) {
                    var pd = System.IO.Directory.GetParent(cd);
                    cd = pd.FullName;
                }
                return cd;
            }
        }
    }
}
