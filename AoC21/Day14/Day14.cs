namespace AoC21;

public class Day14Test
{
    record Rule( string Pair, char Element ) {} 

    static Rule ParseRule( string line ) {
        var p = line.Split( " -> " );
        return new Rule( p[0], p[1].First() );
    }

    static IEnumerable<( string Pair, long Count) > Split( string pair, long count, IDictionary<string,Rule> rules ) {
        var isPair = rules.TryGetValue( pair, out var rule );
        if ( isPair ) {
            yield return ( $"{pair[0]}{rule.Element}", count);
            yield return ( $"{rule.Element}{pair[1]}", count);
        } else yield return ( pair, count );
    }

    static IDictionary<string,long> NextD( IDictionary<string,long> polymer, IDictionary<string,Rule> rules ) 
        => polymer.SelectMany( pair => Split( pair.Key, pair.Value, rules ) )
         .GroupBy( _ => _.Item1 )
         .ToDictionary( _ => _.Key, _ => _.Select( p => p.Item2 ).Sum() );

    static long Result(string start, IDictionary<string,Rule> rules, int steps) {
        IDictionary<string,long> polymer = start.Zip( start.Skip(1) ).Select( p => $"{p.Item1}{p.Item2}" )
            .GroupBy( _ => _ ).ToDictionary( _ => _.Key, _ => _.LongCount() );

        for ( int i = 0; i < steps; i++ ) {
            polymer = NextD(polymer, rules);           
        }
        IEnumerable<(char Ch, long Count)> set = Enumerable.Concat(new[] { (start.First(), 1L) }, polymer.Select(p => (p.Key[1], p.Value)));
        var frequency = set.GroupBy(_ => _.Ch).ToDictionary( _ => _.Key, _ => _.Select( p => p.Item2 ).Sum() );
        var max = frequency.Values.Max();
        var min = frequency.Values.Min();
        return (max - min);
    }

    [TestCase("Day14/input.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var polymer = lines.First();
        var rules = lines.Skip(2).Select( ParseRule ).ToDictionary( _ => _.Pair, _ => _ );
        Result( polymer, rules, 10 ).Should().Be( 2509, "answer 1" );
        Result( polymer, rules, 40 ).Should().Be(2827627697643L, "answer 2" );
    }
}
