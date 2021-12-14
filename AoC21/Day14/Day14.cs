namespace AoC21;

public class Day14Test
{
    record Rule( string Pair, char Element ) {} 

    static Rule ParseRule( string line ) {
        var p = line.Split( " -> " );
        return new Rule( p[0], p[1].First() );
    }
/*
    static string Next1( string polymer, IDictionary<string,Rule> rules ) {
        var parts = polymer.Zip( polymer.Skip(1) )
            .Select( p => { 
                var pair = $"{p.Item1}{p.Item2}"; 
                var isPair = rules.TryGetValue( pair, out var rule );
                return isPair ? $"{rule.Element}{p.Item2}" : $"{p.Item2}";
            } );
        return polymer[0] + string.Concat( parts );
    }
    */

    static IEnumerable<( string Pair, long Count) > Split( string pair, long count, IDictionary<string,Rule> rules ) {
        var isPair = rules.TryGetValue( pair, out var rule );
            if ( isPair ) {
                yield return ( $"{pair[0]}{rule.Element}", count);
                yield return ( $"{rule.Element}{pair[1]}", count);
            }
            yield return ( pair, count );
    }

    static IDictionary<string,long> NextD( IDictionary<string,long> polymer, IDictionary<string,Rule> rules ) 
        => polymer.SelectMany( pair => Split( pair.Key, pair.Value, rules ) )
         .GroupBy( _ => _.Item1 )
         .ToDictionary( _ => _.Key, _ => _.Select( p => p.Item2 ).Sum() );

/*
    static IEnumerable<char> Next( IEnumerable<char> polymer, IDictionary<string,Rule> rules ) {
        var parts = polymer.Zip( polymer.Skip(1) )
            .SelectMany( p => { 
                var pair = $"{p.Item1}{p.Item2}"; 
                var isPair = rules.TryGetValue( pair, out var rule );
                return isPair ? new [] { rule.Element, p.Item2 } : new [] { p.Item2 } ;
            } );
        return Enumerable.Concat( polymer.Take(1), parts );
    }
    */

    static long Result(string start, IDictionary<string,Rule> rules, int steps) {
        /*
        IEnumerable<char> polymer = start;
        for ( int i = 0; i < steps; i++ ) {
            polymer = Next(polymer, rules);           
        }
        var frequency =polymer.GroupBy(_ => _).ToDictionary( _ => _.Key, _ => _.LongCount() );
        var max = frequency.Values.Max();
        var min = frequency.Values.Min();
        return (max - min);
        */

        IDictionary<string,long> polymer = start.Zip( start.Skip(1) ).Select( p => $"{p.Item1}{p.Item2}" )
            .GroupBy( _ => _ ).ToDictionary( _ => _.Key, _ => _.LongCount() );

        for ( int i = 0; i < steps; i++ ) {
            polymer = NextD(polymer, rules);           
        }
        var frequency = Enumerable.Concat( new [] { ( start.First(), 1L ) },
                polymer.Select( p => ( p.Key[1], p.Value ) ) )
            .GroupBy(_ => _).ToDictionary( _ => _.Key, _ => _.LongCount() );
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
        Result( polymer, rules, 40 ).Should().Be( -1, "answer 2" ); //  2691399
    }
}