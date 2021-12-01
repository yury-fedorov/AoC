namespace AoC21.Day01;

public class Day01Test
{
    [TestCase("Day01/input.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var values = lines.Select( int.Parse ).ToArray();
        int? prev = null;
        int count = 0;
        foreach ( var value in values ) {
            count += prev != null &&  value > prev ? 1 : 0;
            prev = value;
        }
        count.Should().Be(1301, "answer 1");

        prev = null;
        count = 0;
        var n = lines.Count() - 2;
        for( var i = 0; i < n; i++ ) {
            int value = values[i] + values[i+1] + values[i+2];
            count += prev != null &&  value > prev ? 1 : 0;
            prev = value;
        }
        count.Should().Be(0, "answer 2");
    }
}