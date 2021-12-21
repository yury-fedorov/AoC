using LanguageExt;

namespace AoC21;

public class Day21Test
{

    static int NextDice( int dice ) => dice >= 100 ? 1 : dice + 1;

    /*
    static int NextDiceTimes( int dice, int times ) {
        for ( int i = 0; i < times; i++ )
            dice = NextDice(dice);
        return dice;
    }
    */

    static int NewPosition( int oldPosition, int delta ) {
        var newPosition = oldPosition + delta;
        while ( newPosition > 11 ) newPosition -= 10; // ie. 11 -> 1
        return newPosition;
    }

    static (int Delta, int Dice) Roll3( int dice ) {
        int delta = 0;
        for ( int i = 0; i < 3; i++ )  {
            delta += dice;
            dice = NextDice(dice);
        }
        return ( delta, dice );
    }

    // [TestCase(7,5)] // my case
    [TestCase(4,8)] // sample
    public void TestGame(int player1, int player2) {
        var dice = 1;
        var points = new [] {0, 0};
        var positions = new [] {player1, player2};
        var playerIndex = 0; // Player 1 goes first.

        var times = 0;
        while ( points.Max() < 1000 ) {
            var result = Roll3( dice );
            // Console.WriteLine( $"{result.Delta} {result.Dice}" );
            var newPosition = NewPosition( positions[playerIndex], result.Delta );
            points[playerIndex] += newPosition;
            positions[playerIndex] = newPosition;
            playerIndex = playerIndex == 0 ? 1 : 0;
            times++;
            dice = result.Dice;
        }
        Console.WriteLine( $"{times} {points.Min()}");
        var a1 = times * points.Min();

        a1.Should().Be(0, "answer 1"); // 308450
        2.Should().Be(0, "answer 2");
    }
}