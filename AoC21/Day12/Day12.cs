using LanguageExt;

namespace AoC21;

public class Day12Test
{
    [TestCase("Day12/input.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var values = lines.Select( int.Parse ).ToArray();

        1.Should().Be(0, "answer 1");
    }
}