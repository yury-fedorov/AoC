namespace AoC21;

public class Day06Test
{
    record Pair(int Age, long Count) { }
    static IEnumerable<Pair> Next(Pair pair) => pair.Age == 0
        ? new[] { new Pair(6, pair.Count), new Pair(8, pair.Count) }
        : new[] { new Pair(pair.Age - 1, pair.Count) };

    static long Answer(IEnumerable<Pair> dictionary, int days)
    {
        for (int day = 0; day < days; day++)
        {
            dictionary = dictionary.SelectMany(Next)
                .GroupBy(_ => _.Age, (age, agg) => new Pair(age, agg.Select(_ => _.Count).Sum()));
        }
        return dictionary.Select(_ => _.Count).Sum();
    }

    [TestCase("Day06/input.txt")]
    public async Task Test(string file)
    {
        var lines = await App.ReadLines(file);
        var dictionary = lines.First().Split(",").Select(int.Parse).GroupBy(_ => _)
            .Select(_ => new Pair(_.Key, _.Count())).ToList();

        Answer(dictionary, 80).Should().Be(346063, "answer 1");
        Answer(dictionary, 256).Should().Be(1572358335990L, "answer 2");
    }
}