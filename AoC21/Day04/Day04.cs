namespace AoC21;

public static class LinqExt
{
    /// <summary>
    /// Break a list of items into chunks of a specific size
    /// </summary>
    public static IEnumerable<List<T>> Chunk<T>(this IEnumerable<T> source, int chunksize)
    {
        while (source.Any())
        {
            yield return source.Take(chunksize).ToList();
            source = source.Skip(chunksize);
        }
    }
}

public class Day04Test
{
    record Board(List<List<int>> Rows) { }

    static IEnumerable<int> ReadBoardRow(string line)
    {
        var matches = Regex.Matches(line, @"(\d+)\s*");
        foreach (Match match in matches)
        {
            yield return Convert.ToInt32(match.Groups[1].Value);
        }
    }

    static int? Bingo(Board board, int[] numbers)
    {
        var all = new List<System.Collections.Generic.HashSet<int>>();
        var n = board.Rows.Count();
        for (int i = 0; i < n; i++)
        {
            all.Add(board.Rows[i].ToHashSet());
            all.Add(Enumerable.Range(0, n).Select(r => board.Rows[r][i]).ToHashSet());
        }
        for (int i = 0; i < numbers.Length; i++)
        {
            var number = numbers[i];
            foreach (var l in all)
            {
                l.Remove(number);
                if (!l.Any()) return i;
            }
        }
        return null;
    }

    static int Answer(List<Board> boards, int[] numbers, int filter)
    {
        var winner = boards.Select(b => (b, Bingo(b, numbers))).Where(_ => _.Item2 == filter).Single();
        int winningIndex = winner.Item2.Value;
        int winningNumber = numbers[winningIndex];
        var allOnBoard = winner.Item1.Rows.SelectMany(_ => _).ToHashSet();
        var allDrawn = Enumerable.Range(0, winningIndex + 1).Select(_ => numbers[_]).ToHashSet();
        allOnBoard.ExceptWith(allDrawn); // substract from allOnBoard
        var sumRemaining = allOnBoard.Sum();
        return (winningNumber * sumRemaining);
    }

    [TestCase("Day04/input.txt")]
    public async Task Test(string file)
    {
        var lines = await App.ReadLines(file);
        var numbers = lines.Take(1).Single().Split(',').Select(int.Parse).ToArray();
        var boardLines = lines.Skip(2).ToList();

        var boards = boardLines.Select(_ => ReadBoardRow(_).ToList()).Where(_ => _.Any())
            .Chunk(5).Select(rows => new Board(rows.ToList())).ToList();

        var results = boards.Select(b => Bingo(b, numbers)).Where(_ => _.HasValue).Select(_ => _.Value).ToList();
        Answer(boards, numbers, results.Min()).Should().Be(46920, "answer 1");
        Answer(boards, numbers, results.Max()).Should().Be(12635, "answer 2");
    }
}