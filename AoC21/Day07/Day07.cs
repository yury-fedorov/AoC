namespace AoC21;

public class Day07Test
{
    static long Fuel2(int a, int b)
    {
        var dx = Math.Abs(a - b);
        return dx * (dx + 1) / 2;
    }

    static long Cost(IEnumerable<(int crab, int qty)> values, int level, bool isFirst)
        => values.Select(_ => _.qty * (isFirst ? Math.Abs(level - _.crab) : Fuel2(level, _.crab))).Sum();

    static long MinCost(IEnumerable<(int crab, int qty)> values, bool isFirst)
    {
        int min = values.Select(_ => _.crab).Min();
        int count = values.Select(_ => _.crab).Max() - min + 1;
        return Enumerable.Range(min, count).Select(level => Cost(values, level, isFirst)).Min();
    }

    [TestCase("Day07/input.txt")]
    public async Task Test(string file)
    {
        var lines = await App.ReadLines(file);
        var values = lines.First().Split(',').Select(int.Parse).ToArray();
        var vc = values.GroupBy(_ => _, (k, a) => (k, a.Count())).ToList();
        MinCost(vc, true).Should().Be(331067, "answer 1");
        MinCost(vc, false).Should().Be(92881128, "answer 2");
    }
}