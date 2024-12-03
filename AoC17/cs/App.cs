namespace AoC17
{
    public class App
    {
        public static string YearDirectory
        {
            get
            {
                var cd = System.IO.Directory.GetCurrentDirectory();
                while (!cd.EndsWith("AoC17"))
                {
                    var pd = System.IO.Directory.GetParent(cd);
                    cd = pd.FullName;
                }
                return cd;
            }
        }

        public static string Directory => System.IO.Path.Combine(YearDirectory, "cs");
    }
}
