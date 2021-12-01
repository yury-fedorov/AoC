namespace AoC21;

public class Day01Test
{
    static int Count( int [] values, int n, Func<int,int> toValue ) {
        Option<int> prev = None;
        int count = 0;
        for( var i = 0; i < n; i++ ) {
            var value = toValue(i);
            count += prev.Match( 
                Some: p  => value > p ? 1 : 0,
                None: () => 0 
            );
            prev = Some(value);
        }
        return count;
    }

    [TestCase("Day01/input.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var values = lines.Select( int.Parse ).ToArray();

        Count(values, values.Length, (i) => values[i]).Should().Be(1301, "answer 1");

        var f = (int i) => values[i] + values[i+1] + values[i+2];
        Count(values, values.Length - 2, f ).Should().Be(1346, "answer 2");
    }
}