namespace AoC21;

public class Day23Test
{
    enum Pod { A = 1, B = 10, C = 100, D = 1000 }

    record Point(int X, int Y)
    {
        public bool IsHall => Y == 0;
        public bool IsCave => !IsHall;
    }
    static int Distance(Point a, Point b)
        => Math.Abs(a.X - b.X) + a.Y + b.Y; // consider up and down in the same step
    static int Energy(Pod p, int distance) => (int)p * distance;

    record Log(Pod Pod, Point From, Point To)
    {
        public int Energy => Energy(Pod, Distance(From, To));
    }

    static Log Step(Pod p, int x0, int y0, int x1, int y1) => new Log(p, new Point(x0, y0), new Point(x1, y1));

    static readonly IDictionary<Pod, int> PodX
        = new Dictionary<Pod, int> { { Pod.A, 2 }, { Pod.B, 4 }, { Pod.C, 6 }, { Pod.D, 8 } };

    static readonly Pod[] Pods = PodX.Keys.ToArray();

    // x coordinates valid for hall as temporal destination
    static readonly int[] XHall = Enumerable.Range(0, 11).Where(x => GetPod(x) == null).ToArray();

    static int GetCaveX(Pod p) => PodX[p];

    static Pod? GetPod(int x)
    {
        var a = PodX.Where(_ => _.Value == x).ToArray();
        return a.Length > 0 ? a.First().Key : null;
    }

    enum PodPhase { Initial, MovedToHall, InCave };

    record PodState(Pod Pod, Point Location, PodPhase Phase) { }

    static PodState Init(Pod pod, int x, int y) => new PodState(pod, new Point(x, y), PodPhase.Initial);
    static PodState[] Init(Pod p1, Pod p2, Pod p3, Pod p4, int x)
        => new PodState[] { Init(p1, x, 1), Init(p2, x, 2), Init(p3, x, 3), Init(p4, x, 4) };

    static bool IsFinalPosition(IEnumerable<PodState> position) => position.All(_ => _.Phase == PodPhase.InCave);

    static int TotalEnergy(IEnumerable<Log> log) => log.Sum(_ => _.Energy);

    enum CaveState { Close, Open, Filled }

    static CaveState GetCaveState(IEnumerable<PodState> position, Pod podOfCave)
    {
        var x = GetCaveX(podOfCave);
        var insideCave = position.Where(_ => _.Location.X == x).ToList();
        var properTypeCount = insideCave.Count(_ => _.Pod == podOfCave);
        if (properTypeCount == 4) return CaveState.Filled;
        var otherTypesCount = insideCave.Count(_ => _.Pod != podOfCave);
        return otherTypesCount > 0 ? CaveState.Close : CaveState.Open;
    }

    // person on top of every cave
    static IEnumerable<PodState> ActiveTopCaves(IEnumerable<PodState> position)
        => position.Where(_ => _.Location.IsCave && _.Phase == PodPhase.Initial)
        .GroupBy(_ => _.Location.X).Select(_ => _.OrderBy(x => x.Location.Y).First());

    static Pod[] OpenCaves(IEnumerable<PodState> position)
        => Pods.Where(p => GetCaveState(position, p) == CaveState.Open).ToArray();

    // without open caves we may not move inside
    static IEnumerable<PodState> MovableToCaves(IEnumerable<PodState> position)
    {
        var openCaves = OpenCaves(position);
        // in hall or on the top of another cave
        return position.Where(_ => _.Phase != PodPhase.InCave
                && openCaves.Contains(_.Pod)
                && IsPathFree(position, _.Location, EmptyOnTopOpenCave(position, _.Pod)));
    }

    static Point? EmptyOnTopOpenCave(IEnumerable<PodState> position, Pod cave)
    {
        var state = GetCaveState(position, cave);
        if (state != CaveState.Open) return null;
        var x = GetCaveX(cave);
        var y = (position.Where(_ => _.Location.X == x)
            .Select(_ => _.Location.Y).DefaultIfEmpty(5).Min()) - 1;
        return new Point(x, y);
    }

    static bool IsPathFree(IEnumerable<PodState> position, Point from, Point to)
    {
        var allPoints = position.Select(_ => _.Location).ToHashSet();
        allPoints.Remove(from); // sure we need to remove from
        var dx = Math.Sign(to.X - from.X);
        Point p = from;
        if (from.IsCave)
        {
            // first we need to get inside hall
            while (!p.IsHall)
            {
                p = new Point(p.X, p.Y - 1);
                if (allPoints.Contains(p)) return false;
            }
        }
        // now we go till the right x
        for (; p.X != to.X; p = new Point(p.X + dx, 0))
        {
            if (allPoints.Contains(p)) return false;
        }
        if (to.IsCave)
        {
            // now we down to the cave
            do
            {
                p = new Point(p.X, p.Y + 1);
                if (allPoints.Contains(p)) return false;
            } while (p != to);
        }
        return !allPoints.Contains(to);
    }

    record PositionLog(List<PodState> Position, List<Log> Log) { }

    static readonly int MaxTotalEnergy = 47549;

    static float Priority(IEnumerable<PodState> position, IEnumerable<Log> log)
        => 32f / Math.Max(0.1f, log.Count()); //+ TotalEnergy(log) / (float)MaxTotalEnergy;

    static int MinTotalEnergy(IEnumerable<PodState> position_, IEnumerable<Log> log_)
    {
        var queue = new PriorityQueue<PositionLog, float>();
        queue.Enqueue(new PositionLog(position_.ToList(), log_.ToList()), Priority(position_, log_));
        int result = MaxTotalEnergy;
        while (queue.Count > 0)
        {
            var n = queue.Dequeue();
            var position = n.Position;
            var log = n.Log;
            var isEnd = IsFinalPosition(position);
            var totalEnergySoFar = TotalEnergy(log);
            if (isEnd)
            {
                result = Math.Min(result, totalEnergySoFar);
                continue;
            }
            if (totalEnergySoFar >= result) continue; // this is already a not good one

            // all from hall or from top of caves could be moved only to "opened" caves
            var toCaves = MovableToCaves(position).ToList();
            if (toCaves.Any())
            {
                // move everything we can to the final caves
                var p1 = new List<PodState>(position);
                var log1 = new List<Log>(log);
                // sequence does not matter
                foreach (var c in toCaves)
                {
                    p1.Remove(c); // it is not anymore inside position
                    var to = EmptyOnTopOpenCave(p1, c.Pod);
                    log1.Add(new Log(c.Pod, c.Location, to));
                    p1.Add(new PodState(c.Pod, to, PodPhase.InCave));
                }
                queue.Enqueue(new PositionLog(p1, log1), Priority(p1, log1));
                continue;
            }

            // 4 toppest pods on the top of caves to other "open" caves or to hall
            var topOnCaves = ActiveTopCaves(position).ToList();

            var optionLog = XHall.SelectMany(x =>
                topOnCaves.Where(from => IsPathFree(position, from.Location, new Point(x, 0)))
                .Select(s => new Log(s.Pod, s.Location, new Point(x, 0)))).ToList();

            if (!optionLog.Any()) continue; // no solution here

            foreach (var l in optionLog)
            {
                var knownEnergy = totalEnergySoFar + l.Energy;
                if (result <= knownEnergy) continue; // too much energy
                var p1 = new List<PodState>(position);
                var log1 = new List<Log>(log);
                var c = p1.Single(_ => _.Location == l.From);
                p1.Remove(c); // it is not anymore inside position
                log1.Add(l);
                p1.Add(new PodState(c.Pod, l.To, PodPhase.MovedToHall));
                queue.Enqueue(new PositionLog(p1, log1), Priority(p1, log1));
            }
        }
        return result;
    }

    [Test]
    public void Test()
    {
        // solved analytically
        var a1log = new[] { Step( Pod.A, 4,1, 0,0 ), Step( Pod.A, 6, 1, 1, 0 ),
            Step( Pod.B, 6,2, 3,0 ), Step( Pod.C, 4,2, 6,2 ), Step( Pod.B, 3,0, 4,2 ),
            Step( Pod.B, 2,1, 4,1 ), Step( Pod.D, 8,1, 9,0 ), Step( Pod.C, 8,2, 6,1 ),Step( Pod.D, 9,0, 8,2 ), Step( Pod.D, 2,2, 8,1 ),
            Step( Pod.A, 0,0, 2,2 ), Step( Pod.A, 1,0, 2,1 )
        };
        TotalEnergy(a1log).Should().Be(15237, "answer 1");

        if (App.IsFast) return; // 1 min 15 sec
        // solution
        var c2 = Init(Pod.B, Pod.D, Pod.D, Pod.D, 2);
        var c4 = Init(Pod.A, Pod.C, Pod.B, Pod.C, 4);
        var c6 = Init(Pod.A, Pod.B, Pod.A, Pod.B, 6);
        var c8 = Init(Pod.D, Pod.A, Pod.C, Pod.C, 8);
        IEnumerable<PodState> input = c2.Union(c4).Union(c6).Union(c8).ToList();
        MinTotalEnergy(input, new Log[] { }).Should().Be(47509, "answer 2");

        return;
        // solved analytically (approximation)
        var a2log = new[] {
            Step( Pod.A, 4,1, 0,0 ),
            Step( Pod.A, 6,1, 1,0 ), // opening C
            Step( Pod.B, 6,2, 3,0 ),
            Step( Pod.A, 6,3, 10,0 ),
            Step( Pod.B, 6,4, 9,0 ),
            Step( Pod.C, 4,2, 6,4 ), // opening B
            Step( Pod.B, 4,3, 7,0 ),
            Step( Pod.C, 4,4, 6,3 ),
            Step( Pod.B, 7,0, 4,4 ), // closing B
            Step( Pod.B, 3,0, 4,3 ),
            Step( Pod.B, 9,0, 4,2 ),
            Step( Pod.B, 2,1, 4,1 ), // B closed
            Step( Pod.D, 8,1, 5,0 ), // opening D
            Step( Pod.A, 8,2, 9,0 ),
            Step( Pod.C, 8,3, 6,2 ),
            Step( Pod.C, 8,4, 6,1 ), //  D open

            Step( Pod.D, 5,0, 8,4 ), // closing D
            Step( Pod.D, 2,2, 8,3 ),
            Step( Pod.D, 2,3, 8,2 ),
            Step( Pod.D, 2,4, 8,1 ),

            Step( Pod.A, 1,0, 2,4 ), // closing A
            Step( Pod.A, 0,0, 2,3 ),
            Step( Pod.A, 9,0, 2,2 ),
            Step( Pod.A, 10,0, 2,1 )
        };
        TotalEnergy(a2log).Should().Be(0, "answer 2"); // 47549 it is too high
    }
}
