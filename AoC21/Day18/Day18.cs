namespace AoC21;

public class Day18Test
{
    record Option( Pair? Pair, int? Value ) { public bool IsValue => Value != null; }
    record Pair( Option Left, Option Right ) { public bool IsSimple => Left.IsValue && Right.IsValue; }
    static long Magnitude(Option pair) => pair.Value != null ? pair.Value.Value 
        : pair.Pair != null ? Magnitude( pair.Pair ) : throw new Exception( "bad pair");
    static long Magnitude(Pair pair) => ( 3 * Magnitude( pair.Left ) ) + ( 2 * Magnitude( pair.Right ) );

    static Option MakeO(int value) => new Option(null, value);
    static Option MakeO(Pair pair) => new Option(pair, null);
    static Pair Make(int Left, int Right) => new Pair(MakeO(Left), MakeO(Right));
    static Pair Make(Pair Left, int Right) => new Pair(MakeO(Left), MakeO(Right));
    static Pair Make(int Left, Pair Right) => new Pair(MakeO(Left), MakeO(Right));
    static Pair Make(Pair Left, Pair Right) => new Pair(MakeO(Left), MakeO(Right));

    [TestCase("[9,1]", 29)]
    [TestCase("[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]", 3488)]
    [TestCase("[[[[6,6],[7,6]],[[7,7],[7,0]]],[[[7,7],[7,7]],[[7,8],[9,9]]]]", 4140)]
    public void TestMagnitude(string pair, int magnitude) => Magnitude(ParsePair(pair)).Should().Be(magnitude);

    static bool IsPair(string text) => text.StartsWith('[') && text.EndsWith(']') && text.Contains(',');

    static bool IsValue(char text) => text switch { >= '0' and <= '9' => true, _ => false };

    enum Phase { Start, Term1, Term2 }

    static Option ParseOption( string text ) => IsPair(text) ? MakeO( ParsePair(text) ) : MakeO( int.Parse(text) );

    static int MaxNested( Pair p ) => p.IsSimple ? 0 
        : 1 + Math.Max( p.Left.Pair  != null ? MaxNested( p.Left.Pair  ) : 0, 
                        p.Right.Pair != null ? MaxNested( p.Right.Pair ) : 0 );
    static bool IsNestedInside4Pairs( Pair p ) => MaxNested(p) >= 4;

    [TestCase("[[[[[9,8],1],2],3],4]", true)]
    public void TestNested(string pair, bool is4 ) => IsNestedInside4Pairs(ParsePair(pair)).Should().Be(is4);


    [TestCase("[[[[[9,8],1],2],3],4]", 4)]
    public void TestMaxNested(string pair, int max ) => MaxNested(ParsePair(pair)).Should().Be(max);

    static string ToString( Option value ) => $"{value.Value?.ToString() ?? ToString(value.Pair)}";

    static string ToString( Pair pair ) => $"[{ToString(pair.Left)},{ToString(pair.Right)}]";

    static Pair Split(int value) => Make( value / 2, (int)Math.Ceiling(value / 2.0) );

    [TestCase(10, "[5,5]")]
    [TestCase(11, "[5,6]")]
    public void TestSplit( int value, string split ) => ToString(Split(value)).Should().Be(split);

    static bool Is10OrAbove(Option value) => value.IsValue ? value.Value >= 10 : Is10OrAbove(value.Pair); 
    static bool Is10OrAbove(Pair pair) => Is10OrAbove(pair.Left) || Is10OrAbove(pair.Right);

    [TestCase("[[[[0,7],4],[15,[0,13]]],[1,1]]", true)]
    [TestCase("[[[[0,7],4],[5,[0,13]]],[1,1]]", true)]
    [TestCase("[[[[0,7],4],[5,[0,3]]],[1,1]]", false)]
    public void Test10OrAbove( string text, bool isAbove ) => Is10OrAbove( ParsePair(text) ).Should().Be(isAbove);

    static Pair ParsePair(string text) {
        var phase = Phase.Start;
        string leftTerm = string.Empty;
        var term = "";
        var level = 0;
        foreach( char ch in text ) {
            var dLevel = ch switch { '[' => 1, ']' => -1, _ => 0 };
            switch (phase) {
                case Phase.Start:
                    if (ch == '[') phase = Phase.Term1;
                    else throw new Exception( "bad symbol" );
                    break;
                case Phase.Term1:
                    level += dLevel;
                    if ( ch == ',' && level == 0 ) { phase = Phase.Term2; leftTerm = term; term = ""; }
                    else term += ch;
                    break;
                case Phase.Term2:
                    if ( ch == ']' && level == 0 )
                        return new Pair( ParseOption(leftTerm), ParseOption(term) );
                    level += dLevel;
                    term += ch;
                    break;
            }
        }
        throw new Exception( "bad pair" );
    }

    static Pair SumPairs( Pair left, Pair right ) {
        var result = Make( left, right );
        // TODO: reduce a snailfish number
        while ( IsNestedInside4Pairs(result) ) {
            // TODO: the leftmost such pair explodes
            //  To explode a pair, the pair's left value is added to the first regular number to the left of the exploding pair 
            // (if any), and the pair's right value is added to the first regular number to the right of the exploding pair 
            // (if any). Exploding pairs will always consist of two regular numbers. Then, the entire exploding pair is 
            // replaced with the regular number 0.
        }
        while (Is10OrAbove(result)) {
            // TODO: the leftmost such regular number splits
        } 
        return result;
    }

    // [TestCase("Day18/input.txt")]
    [TestCase("Day18/sample.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var values = lines.Select( ParsePair ).ToArray();
        var result = values.Skip(1).Aggregate( values.First(), SumPairs );
        Magnitude(result).Should().Be(0, "answer 1");
    }
}