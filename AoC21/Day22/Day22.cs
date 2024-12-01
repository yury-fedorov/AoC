namespace AoC21;

public class Day22Test
{
    enum Axis { X, Y, Z }
    static readonly Axis[] AxisList = { Axis.X, Axis.Y, Axis.Z };
    record Point(int X, int Y, int Z)
    {
        public int Get(Axis a) => a switch { Axis.X => X, Axis.Y => Y, Axis.Z => Z };
    }
    record Cube(Point Min, Point Max)
    {
        public int Size(Axis a)
        {
            var r = Range(a);
            return r.Max - r.Min + 1;
        }
        public long Volume => AxisList.Aggregate(1L, (a, e) => a * Size(e));
        public Range Range(Axis a) => a switch
        {
            Axis.X => new Range(Min.X, Max.X),
            Axis.Y => new Range(Min.Y, Max.Y),
            Axis.Z => new Range(Min.Z, Max.Z)
        };
    }
    record Range(int Min, int Max)
    {
        public int Length => Max - Min + 1;
    }

    record CubeOn(bool On, Cube Cube) { }

    static CubeOn ParseRange(string line)
    {
        // on x=-36..10,y=-44..1,z=-18..28
        var xyz = line.Split(',').Select(_ => _.Split('=')[1].Split("..").Select(int.Parse).ToArray()).ToArray();
        var cube = new Cube(new Point(xyz[0][0], xyz[1][0], xyz[2][0]),
            new Point(xyz[0][1], xyz[1][1], xyz[2][1]));
        var on = line.Contains("on");
        return new CubeOn(on, cube);
    }

    static Range? Limit(Range r)
    {
        const int Max = 50;
        if (r.Min > Max || r.Max < -Max) return null;
        Func<int, int> limit = (x) => x > Max ? Max : (x < -Max ? -Max : x);
        return new Range(limit(r.Min), limit(r.Max));
    }

    static Cube? Limit(Cube c)
    {
        var x = Limit(new Range(c.Min.X, c.Max.X));
        var y = Limit(new Range(c.Min.Y, c.Max.Y));
        var z = Limit(new Range(c.Min.Z, c.Max.Z));
        if (x == null || y == null || z == null) return null;
        return new Cube(new Point(x.Min, y.Min, z.Min), new Point(x.Max, y.Max, z.Max));
    }

    static IEnumerable<Point> RangePoints(Cube r)
        => Enumerable.Range(r.Min.X, r.Size(Axis.X))
            .SelectMany(x => Enumerable.Range(r.Min.Y, r.Size(Axis.Y))
            .SelectMany(y => Enumerable.Range(r.Min.Z, r.Size(Axis.Z))
            .Select(z => new Point(x, y, z))));

    static Range? Intersect(Range a, Range b)
    {
        var min = Math.Max(a.Min, b.Min);
        var max = Math.Min(a.Max, b.Max);
        return min <= max ? new Range(min, max) : null;
    }

    static Range? Intersect(Cube a, Cube b, Axis axis) => Intersect(a.Range(axis), b.Range(axis));

    static Cube? Intersect(Cube a, Cube b)
    {
        var rx = Intersect(a, b, Axis.X);
        var ry = Intersect(a, b, Axis.Y);
        var rz = Intersect(a, b, Axis.Z);
        if (rx == null || ry == null || rz == null) return null;
        return new Cube(new Point(rx.Min, ry.Min, rz.Min), new Point(rx.Max, ry.Max, rz.Max));
    }

    static void Execute1(bool on, Cube range, HashSet<Point> setOn)
    {
        var limitedCube = Limit(range);
        if (limitedCube == null) return;
        Action<Point> aOn = (p) => setOn.Add(p);
        Action<Point> aOff = (p) => setOn.Remove(p);
        var a = on ? aOn : aOff;
        RangePoints(limitedCube).ToList().ForEach(a);
    }

    [TestCase("Day22/input.txt")]
    // [TestCase("Day22/sample.txt")]
    public async Task Test(string file)
    {
        var lines = await App.ReadLines(file);
        var instructions = lines.Select(ParseRange).ToList();
        if (App.IsFast) return;

        // valid but slow answer 1
        var setOn = new HashSet<Point>();
        instructions.ForEach(i => Execute1(i.On, i.Cube, setOn));
        setOn.Count().Should().Be(603661, "answer 1");

        // solution for part 2
        var cubes = new List<CubeOn>();
        foreach (var i in instructions)
        {
            var add = new List<CubeOn>();
            foreach (var c in cubes)
            {
                var ci = Intersect(c.Cube, i.Cube);
                if (ci != null)
                {
                    var s1 = i.On; // instrucion
                    var s2 = c.On; // cube
                    bool sign = (s1 == s2) ? !s1 : s1;
                    // both on or off -> inverse instruction
                    // instruction on, was off -> on
                    // instruction off, was on -> off
                    add.Add(new CubeOn(sign, ci));
                }
            }
            cubes.AddRange(add);
            if (i.On) cubes.Add(i);
        }
        long a2 = cubes.Sum(c => c.Cube.Volume * (c.On ? 1 : -1));
        a2.Should().Be(1237264238382479L, "answer 2");
    }
}
