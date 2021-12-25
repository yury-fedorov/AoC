namespace AoC21;

public class Day24Test
{
    public interface Expression {
        long Get( IDictionary<char,long> memory );
    }

    public class Constant : Expression {
        readonly long _value;
        public Constant(long value) => _value = value;
        public long Get( IDictionary<char,long> memory ) => _value;
    }

    public class Variable : Expression {
        readonly char _variable;
        public Variable(char variable) => _variable = variable;
        public long Get( IDictionary<char,long> memory ) 
            => memory.TryGetValue(  _variable, out var result ) ? result : 0;
    }

    public class Input {
        string _code;
        public Input( long code ) => Reset(code);
        public bool IsValid => !_code.Contains( '0' );
        public int Next() {
            var next = _code.First() - '0';
            _code = _code.Substring( 1 );
            return next;
        }
        public void Reset( long code ) => _code = Convert.ToString( code );
    }

    public abstract class Operation {
        public readonly char Register;

        protected Operation( char register ) => Register = register;
        public abstract bool Execute( IDictionary<char,long> memory );
    }

    public class Operation2Args : Operation {

        readonly Expression Arg2;
        readonly Func<long,long,long> Calculate;
        readonly Func<long,long,bool> Valid;

        public Operation2Args( char register, Expression arg2, Func<long,long,bool> valid, 
                Func<long,long,long> calculate ) : base( register ) {
            Arg2 = arg2;
            Calculate = calculate;
            Valid = valid;
        }

        public override bool Execute( IDictionary<char,long> memory ) {
            var value2 = Arg2.Get( memory );
            var value1 = memory.TryGetValue(Register, out var arg1 ) ? arg1 : 0;
            // if ( !Valid( value1, value2 ) ) return false;
            var result = Calculate( value1, value2 );
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

    static Operation ParseOperation( string line, Input input ) {
        var terms = line.Split(" ");
        var command = terms[0];
        var register = terms[1].First();
        if ( command == "inp" ) return new Inp( register, input );

        Expression arg2 = int.TryParse( terms[2], out var a2 ) ? new Constant( a2 ) : new Variable( terms[2].First() );
        Func<long,long,long> calculate = command switch {
            "add" => (a,b) => a+b,
            "mul" => (a,b) => a*b,
            "div" => (a,b) => a/b,
            "mod" => (a,b) => a%b,
            "eql" => (a,b) => a == b ? 1 : 0,
        };
        Func<long,long,bool> valid = (a,b) => true;
        if ( command == "div" ) valid = (a,b) => b != 0;
        else if ( command == "mod" ) valid = (a,b) => !( a < 0 || b < 0 );
        return new Operation2Args( register, arg2, valid, calculate);
    }

    static long Run( Operation[] code, IDictionary<char,long> memory )
    {
        foreach (var o in code) o.Execute(memory);
        // var isValid = code.All( o => o.Execute(memory) );
        return memory.TryGetValue('z', out var result ) ? result : 0;
    }

    [TestCase("Day24/input.txt")]
    public async Task Test(string file) {
        if ( App.IsFast ) return; 
        var lines = await App.ReadLines(file);
        const long min = 111_111_111_111_11L;
        const long max = 99999353748738L;
            // 999_999_999_999_99L; 
        // var max = 99994761016988L;
        var input = new Input( min );
        var code = lines.Select( _ => ParseOperation(_, input) ).ToArray();
        long a1 = 0; 
        int j = 2000;
        for ( var i = max; i >= min; i-- )
        {
            var i_s = i.ToString().ToCharArray();
            var zi = System.Array.IndexOf(i_s, '0');
            if (zi >= 0)
            {
                for (var y = zi; y < i_s.Length; y++)
                {
                    i_s[y] = '0';
                }
                i = long.Parse(string.Concat(i_s)) - 1;
                continue;
            }
            input.Reset( i );
            var memory = new Dictionary<char,long>();
            var result = Run( code, memory );
            // var di = i - result.Z;
            // Console.WriteLine( $"{i} {result.Z} {di}" );
            if ( result == 0 ) {
                a1 = i;
                break;
            }
            // i = di;
            // if ( j-- < 0 ) break;
        }
       //  var a = 99999999999970L;
       // var b = 5238982983L;
       // a1 = a - b; // 99994761016987L - too high
       // 99994761016967 5238942352
        a1.Should().Be(-1, "answer 1");

        // Count<Triple>(zip3, f ).Should().Be(1346, "answer 2");
    }
}
