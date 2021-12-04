namespace AoC21;

public class Day01Test
{
    record CountState( int Count, Option<int> Prev ) {}
    static int Count<TE>( IEnumerable<TE> values, Func<TE,int> toValue ) =>
        values.Aggregate( new CountState(0, None), (a,v) => new CountState( 
            a.Count + a.Prev.Match( Some: p  => toValue(v) > p ? 1 : 0, None: () => 0 ),
            Some( toValue(v) ) ) 
        ).Count;

    record Triple( int A, int B, int C ) {}

    [TestCase("Day01/input.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var values = lines.Select( int.Parse ).ToArray();

        Count<int>(values, _ => _ ).Should().Be(1301, "answer 1");

        IEnumerable<Triple> zip3 = values.Zip( values.Skip(1) ).Zip( values.Skip(2) )
        .Select( t => new Triple( t.Item1.Item1, t.Item1.Item2,  t.Item2 ) );

        var f = ( Triple t ) => t.A + t.B + t.C;
        Count<Triple>(zip3, f ).Should().Be(1346, "answer 2");
    }
}