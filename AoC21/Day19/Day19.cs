using System.Collections.ObjectModel;

namespace AoC21;

public class Day19Test
{
    record Point( int X, int Y, int Z ) {}
    record Scanner(int Id, ReadOnlyCollection<Point> Beacons ) {}
    enum Coordinate { X, Y, Z }

    record RotationAxis( Coordinate Axis, bool PositiveDirection ) {}
    record Rotation( RotationAxis X, RotationAxis Y, RotationAxis Z ) {} // to remap coordinates / direction

    static int RotateValue( Point p, RotationAxis r ) => ( r.PositiveDirection ? 1 : -1 ) 
        * r.Axis switch { Coordinate.X => p.X, Coordinate.Y => p.Y, Coordinate.Z => p.Z };

    static Point Rotate( Point p, Rotation r ) 
        => new Point( RotateValue( p, r.X ), RotateValue( p, r.Y ), RotateValue( p, r.Z ) );

    record Range( int Min, int Max ) { public int Size => Max - Min; }

    static Range MinMax( IEnumerable<int> values ) => new Range ( values.Min(), values.Max() );

    record Range3D( Range X, Range Y, Range Z ) {}

    static Range3D GetRange( IEnumerable<Point> points ) => new Range3D( 
        MinMax(points.Select( p => p.X )), 
        MinMax(points.Select( p => p.Y )), 
        MinMax(points.Select( p => p.Z )) );

    static Point Shift( Point p, Point s ) => new Point( p.X + s.X, p.Y + s.Y, p.Z + s.Z );

    record MatchResult( HashSet<Point> Set, Point ABShift, Rotation Rotation ) {}

    static MatchResult Match( Scanner a, Scanner b, Rotation r ) {
        var sa = a.Beacons.ToHashSet();
        var sb = b.Beacons.Select( p => Rotate(p, r) ).ToHashSet();
        var ra = GetRange(sa);
        var rb = GetRange(sb);
        var sx = rb.X.Min - ra.X.Min; // b - a = s => b = s + a
        var sy = rb.Y.Min - ra.Y.Min;
        var sz = rb.Z.Min - ra.Z.Min;
        var shift = new Point(sx, sy, sz);
        var sbShifted = sb.Select( p => Shift(p, shift) ).ToHashSet(); // now with the shift of a
        sa.IntersectWith( sbShifted );
        return new MatchResult( sa, shift, r );
    }

    static MatchResult? Match(Scanner a, Scanner b) 
        => RotationOptions().Select( r => Match(a,b,r) ).FirstOrDefault( mr => mr.Set.Count >= 12 );

    static readonly bool [] TRUE_FALSE = new [] {true,false};
    static readonly Coordinate [] XYZ = new [] { Coordinate.X, Coordinate.Y, Coordinate.Z };

    static IEnumerable<RotationAxis> RotationAxisOptions( Coordinate c ) 
        => TRUE_FALSE.Select( d => new RotationAxis(c, d) );

    static Coordinate Last( Coordinate x, Coordinate y ) => XYZ.Single( _ => _ != x && _ != y );
    
    static IEnumerable<Rotation> RotationOptions() {
        var result = new List<Rotation>();
        foreach ( var x in XYZ ) {
            foreach ( var xa in RotationAxisOptions( x ) ) {
                foreach ( var y in XYZ ) {
                    if ( x == y ) continue;
                    var z = Last(x,y);
                    foreach ( var ya in RotationAxisOptions( y ) ) {
                        foreach (  var za in RotationAxisOptions( z ) ) 
                            result.Add( new Rotation( xa, ya, za ) );
                    }
                }
            }
        }
        return result;
    }

    static List<Scanner> ParseInput(string [] lines) {
        var scanners = new List<Scanner>();
        int? id = null;
        var beacons = new List<Point>();
        const string SCANNER = "--- scanner ";
        foreach ( var line in lines.Concat( new [] { string.Empty } ) ) {
            if ( line.StartsWith(SCANNER) ) {
                id = int.Parse( line.Split(SCANNER)[1].Split(' ')[0] );
            } else if ( string.IsNullOrWhiteSpace( line ) ) {
                scanners.Add( new Scanner( id.Value, beacons.AsReadOnly() ) );
                id = null;
                beacons = new List<Point>();
            } else {
                var a = line.Split(',').Select( int.Parse ).ToArray();
                beacons.Add( new Point( a[0], a[1], a[2]) );
            }
        }
        return scanners;
    }

    record Binding( int A, int B, MatchResult Result ) {}

    [TestCase("Day19/input.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var scanners = ParseInput(lines);
        var bindings = new List<Binding>();
        foreach ( var ai in Enumerable.Range( 0, scanners.Count ) ) {
            foreach ( var bi in Enumerable.Range( 0, scanners.Count ) ) {
                if ( bi <= ai ) continue;
                var mr = Match( scanners[ai], scanners[bi] );
                if ( mr != null ) bindings.Add( new Binding( ai, bi, mr ) );
            }
        }

        bindings.Count().Should().Be(-1, "answer 1");
    }
}