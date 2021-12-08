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

    static bool is1(string digit) => digit.Length == 2;  // bgc cg gbdfcae gcbe
    static bool is7(string digit) => digit.Length == 3;
    static bool is4(string digit) => digit.Length == 4;
    static bool is8(string digit) => digit.Length == 7;

    static bool IsEasy(string d) =>  is1(d) || is7(d) || is4(d) || is8(d);

    static bool Contains( string probe, string known, int? count = null ) {
        if ( count == null ) count = known.Length;
        var a = probe.ToCharArray().ToHashSet();
        var b = known.ToCharArray().ToHashSet();
        a.IntersectWith(b);
        return a.Count == count;
    }

    static int ReadCode( List<string> Left, string [] Four ) {
        var digits = new Dictionary<string,char> ();
        var one = Left.Where( is1 ).Single();
        var four = Left.Where( is4 ).Single();
        var seven = Left.Where( is7 ).Single();
        digits.Add( one, '1' );
        digits.Add( seven, '7' );
        digits.Add( four, '4' );
        digits.Add( Left.Where( is8 ).Single(), '8' );
        // beacf afbd bcead cgefa ecdbga efb gbfdeac ecgfbd acbdfe fb | bf efb bgecdfa egcfa
        // fb - 1
        // efb - 7 (e above)
        // afbd - 4 (ad specific for 4 from 1)
        // gbfdeac - 8 (does not help)

        // 0, 6, 9 - 6
        // 2, 3, 5 - 5
        var sixParts = Left.Where( _ => _.Length == 6 ).ToList();
        var fiveParts = Left.Where( _ => _.Length == 5 ).ToList();

        // 9 - contains 1 and 4 and 6 elements
        var nine = sixParts.Single( _ => Contains( _, one ) && Contains( _, four ) );
        // 0 - contains 1 and NOT 4 and 6 elements
        var zero = sixParts.Single( _ => Contains( _, one ) && !Contains( _, four ) );
        // 6 - all rest of 6 elements
        sixParts.Remove( nine );
        sixParts.Remove( zero );
        var six = sixParts.Single();

        // 3 - contains 7 and 5 elements
        var three = fiveParts.Single( _ => Contains(_, seven ) );
        // 5 - contains 3 elements of 4 and 5 elements
        var five = fiveParts.Single( _ => Contains( _, four, 3 ) && !Contains(_, seven ) );
        // 2  - all rest of 5 elements
        fiveParts.Remove( three );
        fiveParts.Remove( five );
        var two = fiveParts.Single();

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

        // In the output values, how many times do digits 1, 4, 7, or 8 appear?
        var a1 = pairs.SelectMany( _ => _.Four ).Where( IsEasy ).Count();
        a1.Should().Be(488, "answer 1");

        var a2 = pairs.Select( _ => ReadCode( _.Left.ToList(), _.Four ) ).Sum();
        a2.Should().Be(0, "answer 2");
    }
}