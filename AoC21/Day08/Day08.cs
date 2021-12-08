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
        if ( count == null ) count = known.Length;
        var a = probe.ToCharArray().ToHashSet();
        a.IntersectWith(known.ToCharArray());
        return a.Count == count;
    }

    static int ReadCode( List<string> Left, string [] Four ) {
        var digits = new Dictionary<string,char> ();
        var one = Left.Single( Is1 );
        var four = Left.Single( Is4 );
        var seven = Left.Single( Is7 );
        digits.Add( one, '1' );
        digits.Add( seven, '7' );
        digits.Add( four, '4' );
        digits.Add( Left.Single( Is8 ), '8' );

        var sixParts = Left.Where( _ => _.Length == 6 ).ToList(); // 0, 6, 9
        var fiveParts = Left.Where( _ => _.Length == 5 ).ToList(); // 2, 3, 5 

        var nine = sixParts.Single( _ => Contains( _, one ) && Contains( _, four ) );
        var zero = sixParts.Single( _ => Contains( _, one ) && !Contains( _, four ) );
        var six = sixParts.Single( _ => _ != nine && _ != zero );

        var three = fiveParts.Single( _ => Contains(_, seven ) );
        var five = fiveParts.Single( _ => Contains( _, four, 3 ) && !Contains(_, seven ) );
        var two = fiveParts.Single( _ => _ != three && _ != five );

        digits.Add( nine, '9' );
        digits.Add( zero, '0' );
        digits.Add( six, '6' );
        digits.Add( three, '3' );
        digits.Add( five, '5' );
        digits.Add( two, '2' );

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