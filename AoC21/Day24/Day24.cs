namespace AoC21;

public class Day24Test
{
    public interface Expression
    {
        long Get(IDictionary<char, long> memory);
    }

    public class Constant : Expression
    {
        public readonly long Value;
        public Constant(long value) => Value = value;
        public long Get(IDictionary<char, long> memory) => Value;
    }

    public class Variable : Expression
    {
        readonly char _variable;
        public Variable(char variable) => _variable = variable;
        public long Get(IDictionary<char, long> memory)
            => memory.TryGetValue(_variable, out var result) ? result : 0;
    }

    public class Input
    {
        string _code;
        public Input(long code) => Reset(code);
        public bool IsValid => !_code.Contains('0');
        public int Next()
        {
            var next = _code.First() - '0';
            _code = _code.Substring(1);
            return next;
        }
        public void Reset(long code) => _code = Convert.ToString(code);
    }

    public abstract class Operation
    {
        public readonly char Register;

        protected Operation(char register) => Register = register;
        public abstract bool Execute(IDictionary<char, long> memory);
    }

    public class Operation2Args : Operation
    {

        public readonly Expression Arg2;
        readonly Func<long, long, long> Calculate;
        readonly Func<long, long, bool> Valid;

        public Operation2Args(char register, Expression arg2, Func<long, long, bool> valid,
                Func<long, long, long> calculate) : base(register)
        {
            Arg2 = arg2;
            Calculate = calculate;
            Valid = valid;
        }

        public override bool Execute(IDictionary<char, long> memory)
        {
            var value2 = Arg2.Get(memory);
            var value1 = memory.TryGetValue(Register, out var arg1) ? arg1 : 0;
            // if ( !Valid( value1, value2 ) ) return false;
            var result = Calculate(value1, value2);
            memory[Register] = result;
            return true;
        }
    }

    public class Inp : Operation
    {
        readonly Input _input;
        public Inp(char register, Input input) : base(register)
        {
            _input = input;
        }

        public override bool Execute(IDictionary<char, long> memory)
        {
            memory[Register] = _input.Next();
            return true;
        }
    }

    static Operation ParseOperation(string line, Input input)
    {
        var terms = line.Split(" ");
        var command = terms[0];
        var register = terms[1].First();
        if (command == "inp") return new Inp(register, input);

        Expression arg2 = int.TryParse(terms[2], out var a2) ? new Constant(a2) : new Variable(terms[2].First());
        Func<long, long, long> calculate = command switch
        {
            "add" => (a, b) => a + b,
            "mul" => (a, b) => a * b,
            "div" => (a, b) => a / b,
            "mod" => (a, b) => a % b,
            "eql" => (a, b) => a == b ? 1 : 0,
        };
        Func<long, long, bool> valid = (a, b) => true;
        if (command == "div") valid = (a, b) => b != 0;
        else if (command == "mod") valid = (a, b) => !(a < 0 || b < 0);
        return new Operation2Args(register, arg2, valid, calculate);
    }

    static long Run(Operation[] code, IDictionary<char, long> memory)
    {
        foreach (var o in code) o.Execute(memory);
        // var isValid = code.All( o => o.Execute(memory) );
        return memory.TryGetValue('z', out var result) ? result : 0;
    }

    // fast solution
    static int X(long z, long w, long xk) => ((z % 26) + xk) != w ? 1 : 0;

    static long ZShort(long z, long w, CodeConst k)
    {
        var vx = X(z, w, k.X);
        return (z / k.Z) * ((25 * vx) + 1) + ((w + k.Y) * vx);
    }

    static long? RunShort(List<CodeConst> cc, Input input)
    {
        long z = 0;
        foreach (var k in cc)
        {
            if (z < 0) return null;
            z = ZShort(z, input.Next(), k);
        }
        return z;
    }

    // inverse formula

    // attempting to execute mod with a<0 or b<=0 will cause the program to crash
    // mod z 26
    static bool IsGood(long z, long w, long X) => z >= 0 && (((z % 26) + X) == w);

    record WZ01(long W, long Z0, long Z1) { }

    static IEnumerable<WZ01> Solutions(CodeConst k, long z1)
    {
        var solutions = new List<WZ01>();
        foreach (var w in Enumerable.Range(1, 9))
        {
            foreach (var z0 in Enumerable.Range(0, (int)ZMax))
            {
                if (ZShort(z0, w, k) == z1)
                {
                    solutions.Add(new WZ01(w, z0, z1));
                }
            }
        }
        return solutions;
    }

    // w to z
    static Dictionary<long, Sequence> Solutions_W_Z0(CodeConst k, long z1)
    {
        var wToZ0 = Enumerable.Range(1, 9)
            .Select(w =>
            {
                var z = w - k.X;
                var ok = IsGood(z, w, k.X) && (ZShort(z, w, k) == z1);
                return ((long)w, z, ok);
            }).ToList();
        return wToZ0.Where(_ => _.Item3).ToDictionary(_ => _.Item1, _ => new Sequence(_.Item2));
    }

    static readonly Dictionary<(long W, long Z1), Sequence> NoAnswer = new();
    static Dictionary<(long W, long Z1), Sequence> Solutions_W_Z0(CodeConst k, Sequence z1)
        => z1.Values.Select(_ => (_, Solutions_W_Z0(k, _))).Where(_ => _.Item2.Any())
            .Select(_ =>
            {
                var z1 = _.Item1;
                var d = _.Item2;
                return d.Select(_ => ((_.Key, z1), _.Value)).ToDictionary(_ => _.Item1, _ => _.Item2);
            }).DefaultIfEmpty(NoAnswer).FirstOrDefault();

    record CodeConst(long X, long Y, long Z) { }  // div z Z (line 5), add x X (line 6), add y Y (line 16)

    record SolutionKey(int Digit, long W, long Z1) { }

    static IEnumerable<SolutionKey> FilterAnswers(IDictionary<SolutionKey, List<long>> raw,
        int digit, long z0) => raw
        .Where(t => t.Key.Digit == digit && t.Value.Contains(z0))
        .Select(t => t.Key)
        .Distinct()
        .ToList();

    static void ReadSolutions(long z0, IDictionary<SolutionKey, List<long>> raw, List<long> solution,
        List<string> solutions, bool isMax)
    {
        int digit = solution.Count;
        if (digit >= 14)
        {
            // the end
            var s = string.Concat(solution.Select(w => (char)((int)(w) + '0')));
            solutions.Add(s);
            return;
        }

        var options = FilterAnswers(raw, digit, z0).ToList();
        var filter = isMax ? options.Max(_ => _.W) : options.Min(_ => _.W);
        foreach (var ii in options.Where(_ => _.W == filter))
        {
            var s = new List<long>(solution);
            s.Add(ii.W);
            ReadSolutions(ii.Z1, raw, s, solutions, isMax);
        }
    }

    // we have always sequence of period 26 due to: (z%26) + k.X == w 
    record Sequence(long V0)
    {
        const int Size = 4000; // approximation
        public IEnumerable<long> Values => Enumerable.Range(0, Size).Select(k => V0 + (26 * k));
        public bool Contain(long test) => Values.Contains(test);
    }

    private const long ZMax = 10_000;

    static bool IsValidZ(long z) => z >= 0 && z <= ZMax;

    [TestCase("Day24/input.txt")]
    public async Task Test(string file)
    {
        if (App.IsFast) return; // 1 min 13 sec
        var lines = await App.ReadLines(file);
        const long min = 111_111_111_111_11L;
        var input = new Input(min);
        var code = lines.Select(_ => ParseOperation(_, input)).ToArray();

        const long max = 99999353748738L;
        var cc = new List<CodeConst>();
        const int Digits = 14;
        var CodePeriod = lines.Length / Digits; // composed of 14 equals chunks

        Func<Operation2Args, long> getConst = (o) => (o.Arg2 as Constant).Value;

        for (var i = 0; i < Digits; i++)
        {
            var offset = i * CodePeriod;
            var inp = code[offset];
            if (!(inp is Inp)) throw new Exception("unexpected code format");
            var z = code[offset + 4] as Operation2Args;
            var x = code[offset + 5] as Operation2Args;
            var y = code[offset + 15] as Operation2Args;

            cc.Add(new CodeConst(getConst(x), getConst(y), getConst(z)));
        }

        var solutions = new Dictionary<SolutionKey, List<long>>(); // value -> Z0 (previous) to bind
        var digit = 13;
        Solutions_W_Z0(cc[digit], 0).ToList()
            .ForEach(_ => solutions.Add(new SolutionKey(digit, _.Key, 0), _.Value.Values.ToList()));

        for (--digit; digit >= 0; digit--)
        {
            var k = cc[digit];
            var d1 = digit + 1;
            var z1list = solutions.Where(_ => _.Key.Digit == d1)
                .SelectMany(p => p.Value)
                .Where(IsValidZ)
                .ToHashSet();
            foreach (var z1 in z1list)
            {
                var sig = Solutions(k, z1).GroupBy(_ => new SolutionKey(digit, _.W, _.Z1)).ToList();
                sig.ForEach(_ => solutions[_.Key] = _.Select(_ => _.Z0).Distinct().ToList());
            }
        }

        const long PA1 = 65984919997939L;
        const long PA2 = 11211619541713L;

        var finalSolutions = new List<string>();
        ReadSolutions(0, solutions, new List<long>(), finalSolutions, true);
        var a1 = finalSolutions.Select(long.Parse).Max();
        while (true)
        {
            input.Reset(a1);
            if (input.IsValid && RunShort(cc, input) == 0L) break;
            a1--;
        }
        a1.Should().Be(PA1, "answer 1");


        finalSolutions.Clear();
        ReadSolutions(0, solutions, new List<long>(), finalSolutions, false);
        var a2 = finalSolutions.Select(long.Parse).Min();
        while (true)
        {
            input.Reset(a2);
            if (input.IsValid && RunShort(cc, input) == 0L) break;
            a2++;
        }
        a2.Should().Be(PA2, "answer 2");
    }
}
