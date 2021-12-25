namespace AoC21;

public class Day22Test
{
    enum Axis {X,Y,Z}
    static readonly Axis[] AxisList = { Axis.X, Axis.Y, Axis.Z };
    record Point( int X, int Y, int Z ) {
        public int Get( Axis a ) => a switch { Axis.X => X, Axis.Y => Y, Axis.Z => Z }; 
    }
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
    record Range( int Min, int Max )
    {
        public int Length => Max - Min + 1;
    }

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

    static bool Within( Range r, Range small ) 
        => Within( r, small.Min ) && Within( r, small.Max );

    static Range? Intersect( Range a, Range b ) {
        var isAOnLeft = a.Min <= b.Min;  // a starts on the left
        var a1 = isAOnLeft ? a : b;
        var b1 = isAOnLeft ? b : a;
        // {}   []
        // {[]}
        // {[}]
        if ( a1.Max >= b1.Min ) {
            // intersec
        } else return null;
        return new Range( b1.Min, Math.Min( a1.Max, b1.Max ) );
    }

    record RangeOn( Range Range, bool On ) {}

    static IEnumerable<RangeOn> Cut(RangeOn a, RangeOn b) {
        var i = Intersect( a.Range, b.Range ); // what is in b
        if ( i != null ) {
            var left = Math.Min( a.Range.Min, b.Range.Min );
            var right = Math.Max( a.Range.Max, b.Range.Max );
            var rl = new Range( left, i.Min - 1 );
            var rr = new Range( 1 + i.Max, right );
            var isBOnLeft = Within( b.Range, rl.Min );
            yield return new RangeOn( rl, isBOnLeft ? b.On : a.On );
            yield return new RangeOn( i, b.On );
            var isBOnRight = Within( b.Range, rr.Min );
            yield return new RangeOn(rr, isBOnRight ? b.On : a.On );
        }
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

        Func<Axis,IEnumerable<RangeOn> > cut = (a) => Cut( 
            new RangeOn( new Range( before.Min.Get(a), before.Max.Get(a) ), true ), 
            new RangeOn( new Range( newCube.Min.Get(a), newCube.Max.Get(a) ), newOn ) )
            .Where( _ => _.Range.Min > _.Range.Max ) // only valid ranges
            .ToArray();

        var x = cut(Axis.X);
        var y = cut(Axis.Y);
        var z = cut(Axis.Z);

        var result = new List<Cube>();
        foreach ( var xi in x ) 
            foreach ( var yi in y )
                foreach ( var zi in z ) {
                    var on = xi.On && yi.On && zi.On;
                    result.Add( new Cube( new Point( xi.Range.Min, yi.Range.Min, zi.Range.Min ),
                        new Point( xi.Range.Max, yi.Range.Max, zi.Range.Max ) ) );
                }

        return result;
    }

    static void Execute1( bool on, Cube range, HashSet<Point> setOn ) {
        var limitedCube = Limit(range);
        if ( limitedCube == null ) return;
        Action<Point> aOn = (p) => setOn.Add(p);
        Action<Point> aOff = (p) => setOn.Remove(p);
        var a = on ? aOn : aOff;
        RangePoints(limitedCube).ToList().ForEach( a );
    }

    // 2,758,514,936,282,235
    // 9,223,372,036,854,775,807

    record Area(Range X, Range Y)
    {
        public long GetArea() => X.Length * Y.Length;
    }

    static bool IsInside( Cube cube, Area area, int z ) 
        => Within( new Range( cube.Min.Z, cube.Max.Z ), z ) 
        && Within( new Range( cube.Min.X, cube.Max.X ), area.X )
        && Within( new Range( cube.Min.Y, cube.Max.Y ), area.Y );

    static List<Area> Areas( HashSet<int> xset, HashSet<int> yset ) {
        var result = new List<Area>();
        result.AddRange(xset.SelectMany(x => yset
            .Select(y => new Area( new Range(x,x), new Range(y,y)))) );
        var xx = xset.Zip( xset.Skip(1) ).ToList();
        var yy = yset.Zip( yset.Skip(1) ).ToList();
        foreach (var xxi in xx)
        {
            yset.Select( y => new Area(new Range(xxi.Item1 + 1, xxi.Item2 - 1), new Range(y, y)))
                .Where( a => a.X.Min <= a.X.Max ).ToList().ForEach( a => result.Add(a) );

            foreach (var yr in yy)
            {
                xx.Select(xr => new Area(new Range(xr.Item1 + 1, xr.Item2 - 1), 
                                                  new Range(yr.Item1 + 1, yr.Item2 - 1)))
                    .Where(a => a.X.Min <= a.X.Max && a.Y.Min <= a.Y.Max).ToList().ForEach(a => result.Add(a));
            }
        }
        foreach (var yyi in yy)
        {
            xset.Select(x => new Area(new Range(x, x), new Range(yyi.Item1 + 1, yyi.Item2 - 1) ))
                .Where(a => a.Y.Min <= a.Y.Max).ToList().ForEach(a => result.Add(a));
        }

        return result;
    }

    [TestCase("Day22/input.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var instructions = lines.Select( ParseRange ).ToList();
        /*
        // valid but slow answer 1
        var setOn = new HashSet<Point>();
        instructions.ForEach( i => Execute1( i.On, i.Cube, setOn ) );
        setOn.Count().Should().Be(603661, "answer 1");
        */
        // var regions = new [] { instructions.First() };
        
        if ( App.IsFast ) return;

        var zset = new HashSet<int>();
        instructions.ForEach( _ => { zset.Add( _.Cube.Min.Z ); zset.Add( _.Cube.Max.Z ); } );
        ;
        var rz = new List<Range>( zset.Select( z => new Range(z,z) ) );
        var oz  = zset.OrderBy(_ => _).ToList();
        oz.Zip( oz.Skip(1) ).Select( _ => new Range( _.Item1 + 1, _.Item2 - 1 ) )
            .Where( r => r.Min <= r.Max ).ToList().ForEach( r => rz.Add(r) );

        long onCount = 0;

        foreach( var rzi in rz )
        {
            var z = rzi.Min;
            var xset = new HashSet<int>();
            var yset = new HashSet<int>();
            foreach (var i in instructions)
            {
                var c = i.Cube;
                if (z >= c.Min.Z && z <= c.Max.Z)
                {
                    xset.Add(c.Min.X); xset.Add(c.Max.X);
                    yset.Add(c.Min.Y); yset.Add(c.Max.Y);
                }
            }
            var areas = Areas(xset, yset);

            var setOn = new Dictionary<Area,bool>();
            // calculate
            foreach (var i in instructions)
            {
                var c = i.Cube;
                areas.Where( a => IsInside( c, a, z ) ).ToList().ForEach( a => setOn[a] = i.On );
            }
            onCount += rzi.Length * setOn.Where(_ => _.Value).Sum(_ => _.Key.GetArea());
        }
        onCount.Should().Be(-2, "answer 2"); // 10967303699759701 - too high - 2 h 50 m
    }
}
