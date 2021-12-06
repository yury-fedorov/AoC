namespace AoC21;

public class Day06Test
{
    record Pair( int Age, long Count ) {}
    static IEnumerable<Pair> Next( Pair pair ) 
        => pair.Age == 0 
        ? new [] { new Pair( 6, pair.Count ), new Pair( 8, pair.Count ) } 
        : new [] { new Pair( pair.Age - 1, pair.Count ) };

    [TestCase("Day06/input.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var dictionary = lines.First().Split(",").Select( int.Parse ).GroupBy( _ => _ )
            .Select( _ => new Pair( _.Key, _.Count() ) );
        Console.WriteLine( string.Join( ',',  dictionary.Select(  p => $"{p.Age} -> {p.Count}" ).ToArray() ) );
        for ( int day = 0; day < 256; day++ ) {
            dictionary = dictionary.SelectMany( Next )
                .GroupBy( _ => _.Age, (age, agg) => new Pair( age, agg.Select(_ => _.Count).Sum() ) );
            //Console.WriteLine( string.Join( ',',  dictionary.Select(  p => $"{p.Age} -> {p.Count}" ).ToArray() ) );
        }
        // 80 // dictionary.Select(_ => _.Count).Sum().Should().Be(346063, "answer 1");

        dictionary.Select(_ => _.Count).Sum().Should().Be(0, "answer 2");
    }
}