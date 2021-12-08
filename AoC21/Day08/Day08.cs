namespace AoC21;

public class Day08Test
{
    static string [] ParseList( string line ) => line.Trim().Split(' ')
        .Select( _ => String.Concat( _.OrderBy(c => c)) ) // order symbols (normalize)
        .ToArray();
    static ( string [] Left, string [] Four ) ParseLine( string line ) {
        //  beacf afbd bcead cgefa ecdbga efb gbfdeac ecgfbd acbdfe fb | bf efb bgecdfa egcfa
        var sides = line.Split( '|' );
        return ( ParseList( sides[0]), ParseList( sides[1] ) );
    }

    static bool Is1(string digit) => digit.Length == 2;
    static bool Is7(string digit) => digit.Length == 3;
    static bool Is4(string digit) => digit.Length == 4;
    static bool Is8(string digit) => digit.Length == 7;
    static bool IsEasy(string d) => Is1(d) || Is7(d) || Is4(d) || Is8(d);

    static bool Contains( string probe, string known, int? count = null ) {
        var a = probe.ToCharArray().ToHashSet();
        a.IntersectWith(known.ToCharArray());
        return a.Count == ( count ?? known.Length );
    }

    static int ReadCode( List<string> Left, string [] Four ) {
        var one = Left.Single( Is1 );
        var four = Left.Single( Is4 );
        var seven = Left.Single( Is7 );
        var eight = Left.Single( Is8 );

        var sixParts = Left.Where( _ => _.Length == 6 ).ToList(); // 0, 6, 9
        var fiveParts = Left.Where( _ => _.Length == 5 ).ToList(); // 2, 3, 5 

        var nine = sixParts.Single( _ => Contains( _, one ) && Contains( _, four ) );
        var zero = sixParts.Single( _ => Contains( _, one ) && !Contains( _, four ) );
        var six = sixParts.Single( _ => _ != nine && _ != zero );

        var three = fiveParts.Single( _ => Contains(_, seven ) );
        var five = fiveParts.Single( _ => Contains( _, four, 3 ) && !Contains(_, seven ) );
        var two = fiveParts.Single( _ => _ != three && _ != five );

        var seq = new [] { zero, one, two, three, four, five, six, seven, eight, nine };
        IDictionary<string,char> digits = seq.Zip( Enumerable.Range( 0, 10 ).Select( n => (char)('0' + n) ) )
            .ToDictionary( _ => _.Item1, _ => _.Item2 );

        var number = String.Concat( Four.Select( d => digits[d] ) );
        return int.Parse(number);
    }
    
    [TestCase("Day08/input.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var pairs = lines.Select( ParseLine ).ToArray();

        var a1 = pairs.SelectMany( _ => _.Four ).Where( IsEasy ).Count();
        a1.Should().Be(488, "answer 1"); // In the output values, how many times do digits 1, 4, 7, or 8 appear?

        var a2 = pairs.Select( _ => ReadCode( _.Left.ToList(), _.Four ) ).Sum();
        a2.Should().Be(1040429, "answer 2");
    }
}