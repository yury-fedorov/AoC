namespace AoC21;

public class Day02Test
{
    enum Direction { Down, Forward, Up }

    static readonly IDictionary<string, Direction> Map = new Dictionary<string, Direction> {
        {@"down (\d+)", Direction.Down}, {@"forward (\d+)", Direction.Forward}, {@"up (\d+)", Direction.Up}
    };

    static (Direction direction, int distance) Read( string line ) {
        foreach ( var map in Map ) {
            var matches = Regex.Matches(line, map.Key);
            foreach (Match match in matches) {
                return ( map.Value, Convert.ToInt32(match.Groups[1].Value) );
            }
        }
        throw new Exception( $"wrong input {line}" );
    }

    [TestCase("Day02/input.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var instructions = lines.Select( Read ).ToArray();

        int x = 0; 
        int y = 0;
        foreach ( var i in instructions ) {
            switch( i.direction ) {
                case Direction.Down: y += i.distance; break;
                case Direction.Up: y -= i.distance; break;
                case Direction.Forward: x += i.distance; break;
            }
        }
        ( x * y ).Should().Be(1698735, "answer 1");

        x = 0; 
        y = 0;
        int aim = 0;
        foreach ( var i in instructions ) {
            switch( i.direction ) {
                case Direction.Down: aim += i.distance; break;
                case Direction.Up: aim -= i.distance; break;
                case Direction.Forward: x += i.distance; y += i.distance * aim; break;
            }
        }
        ( x * y ).Should().Be(1594785890, "answer 2");
    }
}