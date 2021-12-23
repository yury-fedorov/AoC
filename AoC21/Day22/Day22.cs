namespace AoC21;

public class Day22Test
{
    enum Axis {X,Y,Z}
    static readonly Axis[] AxisList = { Axis.X, Axis.Y, Axis.Z };
    record Point( int X, int Y, int Z ) {}
    record Cube( Point Min, Point Max ) {
        public int Size(Axis a) {
            var r = Range(a);
            return r.Max - r.Min + 1;
        } 
        public long Count => AxisList.Aggregate( 1L, (a,e) => a * Size( e ) );
        public Range Range( Axis a ) => a switch {
            Axis.X => new Range( Min.X, Max.X ),
            Axis.Y => new Range( Min.Y, Max.Y ),
            Axis.Z => new Range( Min.Z, Max.Z )
        };
    }
    record Range( int Min, int Max ) {}

    static ( bool On, Cube Cube ) ParseRange( string line ) {
        // on x=-36..10,y=-44..1,z=-18..28
        var xyz = line.Split(',').Select(_ => _.Split('=')[1].Split("..").Select(int.Parse).ToArray()).ToArray();
        var cube = new Cube( new Point( xyz[0][0], xyz[1][0], xyz[2][0] ), 
            new Point( xyz[0][1], xyz[1][1], xyz[2][1] ) );
        var on = line.Contains( "on"  );
        return (on, cube);
    }

    static Range? Limit( Range r ) {
        const int Max = 50;
        if ( r.Min > Max || r.Max < -Max ) return null;
        Func<int,int> limit = (x) => x > Max ? Max : ( x < -Max ? -Max : x );
        return new Range( limit(r.Min), limit(r.Max) );
    }

    static Cube? Limit( Cube c ) {
        var x = Limit( new Range( c.Min.X, c.Max.X ) );
        var y = Limit( new Range( c.Min.Y, c.Max.Y ) );
        var z = Limit( new Range( c.Min.Z, c.Max.Z ) );
        if ( x == null || y == null || z == null ) return null;
        return new Cube( new Point( x.Min, y.Min, z.Min ), new Point( x.Max, y.Max, z.Max ) );
    }

    static IEnumerable<Point> RangePoints( Cube r ) 
        => Enumerable.Range( r.Min.X, r.Size( Axis.X ) )
            .SelectMany( x => Enumerable.Range( r.Min.Y, r.Size( Axis.Y ) )
            .SelectMany( y => Enumerable.Range( r.Min.Z, r.Size( Axis.Z ) )
            .Select( z => new Point( x, y, z ) ) ) );

    static bool Within( Range r, int x ) => x >= r.Min &&  x <= r.Max;
    static (int Min, int Max)? Intersect( Range a, Range b ) {
        var isAOnLeft = a.Min <= b.Min;  // a starts on the left
        var a1 = isAOnLeft ? a : b;
        var b1 = isAOnLeft ? b : a;
        // {}   []
        // {[]}
        // {[}]
        if ( a1.Max >= b1.Min ) {
            // intersec
        } else return null;
        return ( b1.Min, Math.Min( a1.Max, b1.Max ) );
    }

    static bool IsInside( Range big, Range small ) => big.Min <= small.Min && big.Max >= small.Max;

    static bool IsInside( Cube big, Cube small ) {
        if ( small.Count > big.Count ) return false; // does not fit
        return AxisList.All( a => IsInside( big.Range(a), small.Range(a) ) );
    }

    static bool IsNotTouching( Cube a, Cube b ) => AxisList.Any( ax => Intersect( a.Range(ax), b.Range(ax) ) == null );

    static IEnumerable<Cube> Split( Cube before, Cube newCube, bool newOn ) {
        if ( IsInside(before, newCube) ) {
            if ( newOn )
                // before is big enough and in the same state to inglobe the new state inside
                return new [] { before }; 
            
            // we need to make an off "hole" in the before cube
            
        }
        if ( IsNotTouching(before, newCube) )
            return newOn  ? new [] { before, newCube } // no issue they are not touching each other
                          : new [] { before }; // they are not touching and newCube is off
        
        // TODO - implement
        throw new Exception("TODO implement");
    }

    static void Execute1( bool on, Cube range, HashSet<Point> setOn ) {
        var limitedCube = Limit(range);
        if ( limitedCube == null ) return;
        Action<Point> aOn = (p) => setOn.Add(p);
        Action<Point> aOff = (p) => setOn.Remove(p);
        var a = on ? aOn : aOff;
        RangePoints(limitedCube).ToList().ForEach( a );
    }

    [TestCase("Day22/input.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var instructions = lines.Select( ParseRange ).ToList();
        /*
        var setOn = new HashSet<Point>();
        instructions.ForEach( i => Execute1( i.On, i.Cube, setOn ) );
        setOn.Count().Should().Be(603661, "answer 1");
        */
        0.Should().Be(-2, "answer 2");
    }
}