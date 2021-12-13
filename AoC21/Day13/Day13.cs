namespace AoC21;

public class Day13Test
{
    record Point( int X, int Y ) {}
    enum Coordinate { X, Y }
    record Fold ( Coordinate Coordinate, int At ) {}

    static Point ParsePoint(string line) {
        var a = line.Split(',').Select( int.Parse ).ToArray();
        return new Point(a[0], a[1]);
    }

    const string FOLD_ALONG = "fold along ";
    static Fold ParseFold(string line) {
        // fold along x=655
        // fold along y=447
        var a = line.Replace( FOLD_ALONG, "" ).Split( '=' );
        return new Fold( a[0] == "x" ? Coordinate.X : Coordinate.Y, int.Parse(a[1]) );
    }

    static HashSet<Point> ExecuteFold( ICollection<Point> image, Fold fold ) {
        var result = new List<Point>();
        var isX = fold.Coordinate == Coordinate.X;
        result.AddRange( image.Where( p => ( isX ? p.X : p.Y ) <= fold.At ) );
        var max = 2 * fold.At;
        if ( isX ) {
            // from right to left (last row ->  to first row)
            result.AddRange( image.Where( p => p.X > fold.At ).Select( _ => new Point( max - _.X, _.Y ) ) );
        } else {
            // from down to up (last line -> to first line)
            result.AddRange( image.Where( p => p.Y > fold.At ).Select( _ => new Point( _.X, max - _.Y ) ) );
        }
        return result.ToHashSet();
    }

    [TestCase("Day13/input.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var points = lines.TakeWhile( _ => _.Contains(',') ).Select( ParsePoint ).ToArray();
        var folds = lines.SkipWhile( _ => !_.Contains( FOLD_ALONG ) ).Select( ParseFold ).ToArray();

        ExecuteFold( points, folds.First() ).Count().Should().Be(942, "answer 1");

        return;
        var image = points.ToHashSet();
        foreach( var f in folds ) {
            image = ExecuteFold( image, f );
        }
        image.GroupBy( _ => _.Y ).OrderBy( _ => _.Key ).ToList().ForEach(
            _ => {
                var y = _.Key;
                var x = _.Select( _ => _.X ).ToHashSet();
                var line = Enumerable.Range( 0, x.Max() + 1 ).Select( _ => x.Contains(_) ? '*' : ' ' );
                Console.WriteLine( $"{y} {String.Concat(line)}" );
            }
            
        );
        // JZGUAPRB -- printed on the monitor
    }
}