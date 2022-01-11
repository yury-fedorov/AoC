using System.Collections.ObjectModel;

namespace AoC21;

public class Day19Test
{
    record Point(int X, int Y, int Z) { }
    record Scanner(int Id, ReadOnlyCollection<Point> Beacons) { }
    enum Coordinate { X, Y, Z }

    record RotationAxis(Coordinate Axis, bool PositiveDirection) { }
    record Rotation(RotationAxis X, RotationAxis Y, RotationAxis Z) { } // to remap coordinates / direction

    static int RotateValue(Point p, RotationAxis r) => (r.PositiveDirection ? 1 : -1)
        * r.Axis switch { Coordinate.X => p.X, Coordinate.Y => p.Y, Coordinate.Z => p.Z };

    static Point Rotate(Point p, Rotation r)
        => new Point(RotateValue(p, r.X), RotateValue(p, r.Y), RotateValue(p, r.Z));

    record Range(int Min, int Max) { public int Size => Max - Min; }

    static Range MinMax(IEnumerable<int> values) => new Range(values.Min(), values.Max());

    record Range3D(Range X, Range Y, Range Z) { }

    static Range3D GetRange(IEnumerable<Point> points) => new Range3D(
        MinMax(points.Select(p => p.X)),
        MinMax(points.Select(p => p.Y)),
        MinMax(points.Select(p => p.Z)));

    static Point Shift(Point p, Point s) => new Point(p.X - s.X, p.Y - s.Y, p.Z - s.Z);

    record MatchResult(HashSet<Point> Intersect, Point ABShift, Rotation Rotation, HashSet<Point> Shifted ) { }

    static MatchResult Match(Scanner a, Scanner b, Rotation r) {
        var sa = a.Beacons.ToHashSet();
        var sb = b.Beacons.Select(p => Rotate(p, r)).ToHashSet();
        // b - a = s => b = s + a; a = b - s
        Point shift = sa.SelectMany(a => sb.Select(b => Shift(b, a))).GroupBy(_ => _)
            .OrderByDescending(_ => _.Count()).Select( _ => _.Key ).FirstOrDefault();
        var sbShifted = sb.Select(p => Shift(p, shift)).ToHashSet(); // now with the shift of a
        sa.IntersectWith(sbShifted);
        return new MatchResult(sa, shift, r, sbShifted);
    }

    const int SharedPoints = 12;

    static MatchResult? Match(Scanner a, Scanner b)
        => RotateOptions.Select(r => Match(a, b, r)).OrderByDescending(mr => mr.Intersect.Count).First();
    // .FirstOrDefault(mr => mr.Set.Count >= SharedPoints);

    static readonly bool[] TRUE_FALSE = new[] { true, false };
    static readonly Coordinate[] XYZ = new[] { Coordinate.X, Coordinate.Y, Coordinate.Z };

    static IEnumerable<RotationAxis> RotationAxisOptions(Coordinate c)
        => TRUE_FALSE.Select(d => new RotationAxis(c, d));

    static Coordinate Last(Coordinate x, Coordinate y) => XYZ.Single(_ => _ != x && _ != y);

    static readonly Rotation [] RotateOptions = RotationOptions().ToArray();

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

    static int Distance(Point a, Point b) {
        var d = Shift(a, b);
        return Math.Abs(d.X) + Math.Abs(d.Y) + Math.Abs(d.Z);
    }

    [TestCase("Day19/input.txt")]
    // [TestCase("Day19/sample.txt")]
    // [TestCase("Day19/sample1.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var scanners = ParseInput(lines);
        var bindings = new List<Binding>();
        /*
        foreach ( var ai in Enumerable.Range( 0, scanners.Count ) ) {
            foreach ( var bi in Enumerable.Range( 0, scanners.Count ) ) {
                if ( bi <= ai ) continue;
                
                if ( mr != null && mr.Set.Count >= SharedPoints) 
                    bindings.Add( new Binding( ai, bi, mr ) );
            }
        }
        */
        // normalized coordinates
        var map = new Dictionary<int, Scanner>() { { 0, scanners[0] } };
        // to convert
        while ( map.Count < scanners.Count )
        {
            foreach (var si in Enumerable.Range(0, scanners.Count))
            {
                if (map.ContainsKey(si)) continue;
                // only missing in map
                foreach (var mi in map.Keys.ToArray() )
                {
                    var mr = Match(map[mi], scanners[si]);
                    if (mr != null && mr.Intersect.Count >= SharedPoints)
                    {
                        bindings.Add(new Binding(mi, si, mr));
                        map.Add(si, new Scanner(si, mr.Shifted.ToList().AsReadOnly()));
                        break;
                    }
                }
            }
        }
        var beacons = map.Values.SelectMany(s => s.Beacons).ToHashSet();
        beacons.Count().Should().Be(392, "answer 1");

        var a2 = Enumerable.Range(0, scanners.Count).SelectMany(a => Enumerable.Range(0, scanners.Count).Where(b => a < b).Select(b => (a, b)))
            .Select(ab =>
           {
               var a = ab.Item1;
               var b = ab.Item2;
               var ap = bindings.Where(_ => _.B == a).Select(_ => _.Result.ABShift).DefaultIfEmpty(new Point(0, 0, 0)).First();
               var bp = bindings.Where(_ => _.B == b).Select(_ => _.Result.ABShift).DefaultIfEmpty(new Point(0, 0, 0)).First();
               return Distance(ap, bp);
           }).Max();
        a2.Should().Be(13332, "answer 2");
    }
}
