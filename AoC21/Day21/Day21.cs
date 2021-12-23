namespace AoC21;

public class Day21Test
{
    static int NextDice( int dice ) => dice >= 100 ? 1 : dice + 1;

    static int NewSpace( int oldSpace, int delta ) {
        var newSpace = oldSpace + delta;
        while ( newSpace > 10 ) newSpace -= 10;
        return newSpace;
    }

    static (int Delta, int Dice) Roll3( int dice ) {
        int delta = 0;
        for ( int i = 0; i < 3; i++ )  {
            delta += dice;
            dice = NextDice(dice);
        }
        return ( delta, dice );
    }

    private static readonly int[] DiracDice = { 1, 2, 3 };

    private static readonly IDictionary<int, int> ValueTimes = DiracDice.SelectMany(a => DiracDice.SelectMany(b => DiracDice.Select(c => a + b + c)))
        .GroupBy(_ => _).ToDictionary(_ => _.Key, _ => _.Count());

    static (long Victories1, long Victories2) Fast(int space1, int space2, int score1, int score2, bool isFirst, int targetScore)
    {
        var space = isFirst ? space1 : space2;
        var score = isFirst ? score1 : score2;
        long victory1 = 0;
        long victory2 = 0;
        foreach (var rp in ValueTimes)
        {
            var delta = rp.Key;
            var times = rp.Value;
            var newSpace = NewSpace(space, delta);
            var newScore = score + newSpace;
            if ( newScore >= targetScore)
            {
                if (isFirst) victory1 += times;
                else victory2 += times;
            }
            else
            {
                var _space1 = isFirst ? newSpace : space1;
                var _space2 = isFirst ? space2 : newSpace;
                var _score1 = isFirst ? newScore : score1;
                var _score2 = isFirst ? score2 : newScore;
                var r = Fast(_space1, _space2, _score1, _score2, !isFirst, targetScore);
                victory1 += times * r.Victories1;
                victory2 += times * r.Victories2;
            }

        }
        return (victory1, victory2);
    }

    [TestCase(7,5)] // my case
    // [TestCase(4,8)] // sample
    public void TestGame(int player1, int player2) {
        if ( App.IsFast ) return;

        var dice = 1;
        var score = new [] {0, 0};
        var space = new [] {player1, player2};
        var playerIndex = 0; // Player 1 goes first.

        var times = 0;
        while ( score.Max() < 1000 ) {
            var result = Roll3( dice );
            var newSpace = NewSpace(space[playerIndex], result.Delta );
            score[playerIndex] += newSpace;
            space[playerIndex] = newSpace;
            playerIndex = playerIndex == 0 ? 1 : 0;
            times++;
            dice = result.Dice;
        }
        var a1 = 3* times * score.Min();
        a1.Should().Be(798147, "answer 1");

        var a2 = Fast(player1, player2, 0, 0, true, 21);
        Math.Max( a2.Victories1, a2.Victories2 ) .Should().Be(809953813657517L, "answer 2");
    }
}
