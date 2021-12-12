namespace AoC21;

public class Day12Test
{
    record Path( string A, string B ) {}
    const string START = "start";
    const string END = "end";

    static Path ParsePath( string line ) {
        var p = line.Split( '-' );
        return new Path( p[0], p[1] );
    }

    static bool IsLower( string place ) => place.Any( ch => ch >= 'a' && ch <= 'z' );

    static bool IsValid2( IEnumerable<string> path ) {
        var doubled = path.Where( IsLower ).GroupBy(_ => _).Where( _ => _.Count() > 1 ).Select( _ => _.Key ).ToList();
        var n = doubled.Count();
        if ( n > 1 ) return false;
        if ( n == 0 ) return true;
        var dd = doubled.First();
        return dd != START && dd != END;
    }

    static bool IsValid( IEnumerable<string> path, string tail ) => IsLower(tail) ? !path.Contains(tail) : true;

    static List<string> Compose( List<string> path, string tail ) {
        var result = new List<string>( path );
        result.Add( tail );
        return result;
    }

    static HashSet<string> Next( Path[] graph, List<string> path, bool isPart1 ) {
        var last = path.Last();
        return graph.Where( p => p.A == last || p.B == last )
            .Select( p => p.A == last ? p.B : p.A )
            .Where( tail => isPart1 ? IsValid( path, tail ) : IsValid2( Compose(path, tail) ) )
            .ToHashSet();
    }

    static IEnumerable< List<string> > AllPaths( Path [] graph, List<string> path, bool isPart1 ) {
        var result = new List< List< string> > ();
        var partial = new List< List< string> > ();
        while ( true ) {
            var options = Next(graph, path, isPart1);
            foreach ( var last in options ) {
                var path1 = Compose( path, last );
                if ( last == END ) { result.Add( path1 ); }
                else if ( last == START ) {}
                else { 
                    partial.Add( path1 );
                }
            }
            if ( !partial.Any() ) break;
            path = partial.Last();
            partial.RemoveAt(partial.Count() - 1);
        }
        return result;
    }

    // [TestCase("Day12/input.txt")]
     [TestCase("Day12/sample.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var graph = lines.Select( ParsePath ).ToArray();
        var start = new List<string>{ START };
        // AllPaths(graph, start, true ).Count().Should().Be(5920, "answer 1");
        // 
        AllPaths(graph, start, false ).Count().Should().Be(-2, "answer 2");
    }
}