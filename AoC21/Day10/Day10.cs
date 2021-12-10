namespace AoC21;

public class Day10Test
{
    static int Points( char ch ) => ch switch {
        ')' => 3,
        ']' => 57,
        '}' => 1197,
        '>' => 25137,
        _ => throw new Exception( $"Unexpected character {ch}" )
    };

    static readonly char [] Open  = new [] { '(', '[', '{', '<' };
    static readonly char [] Close = new [] { ')', ']', '}', '>' };

    static char GetClose(char open) => Enumerable.Range(0, Open.Length).Select( i => ( i, Open[i] ) )
        .Where( _ => _.Item2 == open ).Select( _ => Close[_.Item1] ).Single();

    static char? Check( string line ) {
        var stack = new Stack<char>();
        foreach( char ch in line ) {
            if ( Open.Any( _ => ch == _ ) ) stack.Push(ch);
            else if ( Close.Any( _ => ch == _ ) ) {
                var chOpen = stack.Pop();
                var expectedClose = GetClose(chOpen);
                if ( expectedClose != ch ) return ch;
            } else throw new Exception( $"Unexpected char: {ch}" );
        }
        return null;
    }

    static IEnumerable<char> Missing( string line ) {
        var stack = new Stack<char>();
        foreach( char ch in line ) {
            if ( Open.Any( _ => ch == _ ) ) stack.Push(ch);
            else if ( Close.Any( _ => ch == _ ) ) stack.Pop();
        }
        while( stack.Any() ) 
        {
            yield return GetClose( stack.Pop() );
        }
    }

    static long PointsMissing( IEnumerable<char> missing ) {
        long total = 0;
        foreach( var ch in missing ) {
            total *= 5;
            var index = Enumerable.Range(0, Close.Length).Select( i => ( i, Close[i] ) ).Single( _ => _.Item2 == ch ).Item1;
            total += index + 1;
        }
        return total;
    }

    [TestCase("Day10/input.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var values = lines.ToArray();

        values.Select( Check ).Where( _ => _ != null ).Cast<char>().Select( Points ).Sum()
            .Should().Be(392421, "answer 1");

        var scores = values.Where( _ => Check(_) == null )
            .Select( _ => Missing(_).ToList() ).Where( _ => _.Any() )
            .Select( PointsMissing ).OrderBy( _ => _ ).ToArray();
        var n = scores.Length;
        scores[(int)(n/2)].Should().Be(2769449099L, "answer 2");
    }
}