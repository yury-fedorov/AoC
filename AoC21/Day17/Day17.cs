namespace AoC21;

public class Day17Test
{
    record Point (int X, int Y) {}
    record Area (Point Min, Point Max) {}

    static (int From, int To) ParseRange( string fromTo ) {
        var pair = fromTo.Split("=")[1].Split("..").Select( int.Parse ).ToArray();
        return MinMax( pair[0], pair[1] );
    }
    static Area ParseArea(string input) {
        var xy = input.Split(", ");
        var rx = ParseRange(xy[0]);
        var ry = ParseRange(xy[1]);
        return new Area( new Point(rx.From, ry.From), new Point( rx.To, ry.To ) );
    }

    static (int Min, int Max) MinMax( int a, int b ) => a < b ? (a,b) : (b,a);

    static bool IsIn( int from, int to, int value ) => from <= value && value <= to;

    static bool IsIn(Area a, Point p) => IsIn( a.Min.X, a.Max.X, p.X ) && IsIn( a.Min.Y, a.Max.Y, p.Y );

    static ( bool In, List<Point> Path, Point Velocity, int MaxY ) IsInArea( Area target, Point velocity ) {
        var v0 = velocity;
        var p = new Point(0,0);
        var path = new List<Point>();
        while ( p.X <= target.Max.X &&  p.Y >= target.Min.Y ) {
            path.Add(p);
            if ( IsIn(target, p) ) return ( true, path, v0, path.Select( p => p.Y ).Max() );
            p = new Point( p.X + velocity.X, p.Y + velocity.Y );
            var vx = Math.Max( 0, velocity.X - 1 );
            var vy = velocity.Y - 1;
            velocity = new Point( vx, vy );
        }
        return ( false, path, v0, int.MinValue );
    }

    // [TestCase("x=20..30, y=-10..-5")] // sample
    [TestCase("x=79..137, y=-176..-117")] // my input
    public async Task Test(string targetArea) {
        var area = ParseArea(targetArea);
        var rx = Enumerable.Range( 1, area.Max.X ).ToArray();
        var ry = Enumerable.Range( area.Min.Y, (2 * area.Max.X - area.Min.Y) ).ToArray();
        var options = rx.SelectMany( x => ry.Select( y => IsInArea( area, new Point(x,y) ) )
            .Where( _ => _.In ) ).ToList();     
        options.Select( _ => _.MaxY ).Max().Should().Be(15400, "answer 1");
        options.Count().Should().Be(5844, "answer 2");
    }
}
