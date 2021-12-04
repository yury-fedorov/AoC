namespace AoC21;

public class Day04Test
{
    record Board( List<int []> Rows ) {}

    static IEnumerable<int> ReadBoardRow( string line ) {
        var matches = Regex.Matches(line, @"(\d+)\s*");
        foreach (Match match in matches) {
            yield return Convert.ToInt32(match.Groups[1].Value);
        }
    }

    static int? Bingo( Board board, int [] numbers ) {
        var all = new List<System.Collections.Generic.HashSet<int>>();
        var n = board.Rows.Count();
        for ( int i = 0; i < n; i++ ) {
            all.Add( board.Rows[i].ToHashSet() );
            all.Add( Enumerable.Range( 0, n ).Select( r => board.Rows[r][i] ).ToHashSet() );
        }
        for ( int i = 0; i < numbers.Length; i++ ) {
            var number = numbers[i];
            foreach ( var l in all ) {
                l.Remove( number );
                if ( !l.Any() ) return i;
            }   
        }
        return null;
    }

    [TestCase("Day04/input.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var numbers = lines.Take(1).Single().Split(',').Select(int.Parse).ToArray();
        var boardLines = lines.Skip(2).ToList();
        boardLines.Add( string.Empty );

        var boards = new List<Board>();
        var rows = new List<int []>(); 

        foreach( var line in boardLines ) {
            var row = ReadBoardRow( line ).ToArray();
            if ( row.Length > 1 ) rows.Add(row);
            else if ( rows.Any() ) {
                boards.Add( new Board( rows ) );
                rows = new List<int []>(); 
            }
        }

        var results = boards.Select( b => Bingo( b, numbers ) ).Where( _ => _.HasValue ).Select( _ => _.Value ).ToList();
        {
            var minIterations = results.Min();
            var winner = boards.Select( b => ( b, Bingo( b, numbers ) ) ).Where( _ => _.Item2 == minIterations ).Single();
            int winningIndex = winner.Item2.Value;
            int winningNumber = numbers[winningIndex];
            var allOnBoard = winner.Item1.Rows.SelectMany( _ => _ ).ToHashSet();
            var allDrawn = Enumerable.Range(0, winningIndex + 1).Select( _ => numbers[_] ).ToHashSet();
            allOnBoard.ExceptWith(allDrawn); // substract from allOnBoard
            var sumRemaining = allOnBoard.Sum();       
            ( winningNumber * sumRemaining ).Should().Be(46920, "answer 1");
        }

        {
            var maxIterations = results.Max();
            var looser = boards.Select( b => ( b, Bingo( b, numbers ) ) ).Where( _ => _.Item2 == maxIterations ).Single();
            
            int looserIndex = looser.Item2.Value;
            int looserWinningNumber = numbers[looserIndex];
            var allOnBoard = looser.Item1.Rows.SelectMany( _ => _ ).ToHashSet();
            var allDrawn = Enumerable.Range(0, looserIndex + 1).Select( _ => numbers[_] ).ToHashSet();
            allOnBoard.ExceptWith(allDrawn); // substract from allOnBoard
            var sumRemaining = allOnBoard.Sum();       
            ( looserWinningNumber * sumRemaining ).Should().Be(12635, "answer 2");
        }
    }
}