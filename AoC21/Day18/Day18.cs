namespace AoC21;

public class Day18Test
{
    abstract class Object {
        public Pair? Pair => this as Pair;
        public int? Number => ( this as Number )?.IntValue;
        public bool IsNumber => this is Number; 
    }
    class Number : Object {
        public int IntValue;
    }
    class Pair : Object { 
        public Object Left;
        public Object Right;
        public Object Get( bool isLeft ) => isLeft ? Left : Right;
        public bool IsSimple => Left.IsNumber && Right.IsNumber; 
    }
    static long Magnitude(Object obj) => obj is Number number ? number.IntValue  : obj is Pair pair ? Magnitude( pair ) 
        : throw new Exception( "bad pair");
    static long Magnitude(Pair pair) => ( 3 * Magnitude( pair.Left ) ) + ( 2 * Magnitude( pair.Right ) );

    static Object MakeO(int value) => new Number { IntValue = value };
    static Object MakeO(Pair pair) => pair;
    static Pair Make(int Left, int Right) => new Pair { Left = MakeO(Left), Right = MakeO(Right) };
    static Pair Make(Pair Left, int Right) => new Pair { Left = MakeO(Left), Right = MakeO(Right) };
    static Pair Make(int Left, Pair Right) => new Pair { Left = MakeO(Left), Right = MakeO(Right) };
    static Pair Make(Pair Left, Pair Right) => new Pair { Left = MakeO(Left), Right = MakeO(Right) };

    [TestCase("[9,1]", 29)]
    [TestCase("[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]", 3488)]
    [TestCase("[[[[6,6],[7,6]],[[7,7],[7,0]]],[[[7,7],[7,7]],[[7,8],[9,9]]]]", 4140)]
    public void TestMagnitude(string pair, int magnitude) => Magnitude(ParsePair(pair)).Should().Be(magnitude);

    static bool IsPair(string text) => text.StartsWith('[') && text.EndsWith(']') && text.Contains(',');

    static bool IsValue(char text) => text switch { >= '0' and <= '9' => true, _ => false };

    enum Phase { Start, Term1, Term2 }

    static Object ParseOption( string text ) => IsPair(text) ? MakeO( ParsePair(text) ) : MakeO( int.Parse(text) );

    static int MaxNested( Pair p ) => p.IsSimple ? 0 
        : 1 + Math.Max( p.Left.Pair  != null ? MaxNested( p.Left.Pair  ) : 0, 
                        p.Right.Pair != null ? MaxNested( p.Right.Pair ) : 0 );
    static bool IsNestedInside4Pairs( Pair p ) => MaxNested(p) >= 4;

    [TestCase("[[[[[9,8],1],2],3],4]", true)]
    public void TestNested(string pair, bool is4 ) => IsNestedInside4Pairs(ParsePair(pair)).Should().Be(is4);


    [TestCase("[[[[[9,8],1],2],3],4]", 4)]
    public void TestMaxNested(string pair, int max ) => MaxNested(ParsePair(pair)).Should().Be(max);

    static string ToString( Object value ) => $"{value.Number?.ToString() ?? ToString(value.Pair)}";

    static string ToString( Pair pair ) => $"[{ToString(pair.Left)},{ToString(pair.Right)}]";

    static Pair Split(int value) => Make( value / 2, (int)Math.Ceiling(value / 2.0) );

    [TestCase(10, "[5,5]")]
    [TestCase(11, "[5,6]")]
    public void TestSplit( int value, string split ) => ToString(Split(value)).Should().Be(split);

    static bool Is10OrAbove(Object value) => value.IsNumber ? value.Number >= 10 : Is10OrAbove(value.Pair); 
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
                    if ( ch == ']' && level == 0 )  {
                        var left = ParseOption(leftTerm);
                        var right = ParseOption(term);
                        var pair = new Pair { Left = left, Right = right };
                        return pair;
                    }
                    level += dLevel;
                    term += ch;
                    break;
            }
        }
        throw new Exception( "bad pair" );
    }
    static Number? LeftMost10OrAbove( Object value ) {
        if (value is Number number ) { if ( number.IntValue >= 10 ) return number; }
        else { if ( Is10OrAbove( value ) ) return LeftMost10OrAbove( value.Pair ); }
        return null;
    }

    static Number? LeftMost10OrAbove( Pair pair ) {
        if ( pair.IsSimple ) return Is10OrAbove(pair) ? ( pair.Left.Number >= 10 ? pair.Left : pair.Right ) as Number : null;
        return LeftMost10OrAbove( pair.Left ) ?? LeftMost10OrAbove( pair.Right );
    }

    static Object? LeftMostExploding( Pair tree, int level = 4 )
    {
        if (MaxNested(tree) < level) return null; // no way
        if (level == 0 ) return tree.IsSimple ? tree : null;
        for ( var dl = 0; dl < 2; dl++ )
        {
            if (tree.Left is Pair lp)
            {
                var ml = LeftMostExploding(lp, level - dl);
                if (ml != null) return ml;
            }
            if (tree.Right is Pair rp)
            {
                var ml = LeftMostExploding(rp, level - dl );
                if (ml != null) return ml;
            }
        }
        return null;
    }

    record ParentResult( Object Parent, bool IsLeft ) {}

    static ParentResult? Parent( Object tree, Object obj ) {
        if ( tree == obj ) return null; // if object is head of the tree, there is no parent
        if ( tree is Pair treeAsPair ) {
            if ( treeAsPair.Left == obj ) return new ParentResult( tree, true );
            if ( treeAsPair.Right == obj ) return new ParentResult( tree, false );
            if ( treeAsPair.Left is Pair pair ) {
                var result = Parent( pair, obj );
                if ( result != null ) return result;
            }
            if ( treeAsPair.Right is Pair rightPair ) return Parent( rightPair, obj );
            return null;
        }
        return null; // if tree is a number and does not match the object (checked already) - no solution
    }

    // left / right access by enum
    // inverse left to right and back
    static Number? FindValue( Pair tree, bool isLeft ) {
        if (tree == null) return null;
        if ( !isLeft ) {
            if ( tree.Left is Number leftNumber ) return leftNumber;
            var left = FindValue( tree.Left.Pair, isLeft );
            if ( left != null ) return left;
            if ( tree.Right is Number rightNumber ) return rightNumber;
            return FindValue( tree.Right.Pair, isLeft );
        } else {
            if ( tree.Right is Number rightNumber ) return rightNumber;
            var right = FindValue( tree.Right.Pair, isLeft );
            if ( right != null ) return right;
            if ( tree.Left is Number leftNumber ) return leftNumber;
            return FindValue( tree.Left.Pair, isLeft );
        }
    }

    static Number? FirstOn( Pair tree, Object exploding, bool isOnLeft ) {
        while ( true )  {
            var parent = Parent(tree, exploding);
            if ( parent == null ) return null; // not found anything
            Pair parentPair = (Pair)parent.Parent;
            if ( parent.IsLeft != isOnLeft ) {  // exploding node is the right node of the parent
                // left side down could be value or tree
                // first down as it is closer then up
                var obj = parentPair.Get(isOnLeft);
                if (obj is Number number) return number;
                var down = FindValue( obj.Pair, isOnLeft );
                if ( down != null ) return down;
            }
            // if we did not find somewhere withing the direct parent of exploding
            // we go one level above (if parent node is right - we search the left one down)
            // if not we skip and go one level above
            exploding = parentPair;
        }
    }

    static Object Replace(Object tree, Object oldObj, Object newObj ) {
        var parent = Parent( tree, oldObj );
        if (parent == null) {
            // is it the head
            if ( tree == oldObj ) return newObj;
            throw new Exception( "parent was not found" );
        }
        Pair parentPair = parent.Parent.Pair;
        if (parentPair.Left == oldObj) parentPair.Left = newObj;
        else parentPair.Right = newObj;
        return tree;
    }

    // reduce a snailfish number
    static Pair ReduceSnailfish( Pair result ) {
        while ( true ) {
            // the leftmost such pair explodes
            //  To explode a pair, the pair's left value is added to the first regular number to the left of the exploding pair 
            // (if any), and the pair's right value is added to the first regular number to the right of the exploding pair 
            // (if any). Exploding pairs will always consist of two regular numbers.
            var exploding = LeftMostExploding(result); // option that is always a pair
            if ( exploding != null )
            {
                Pair pair = exploding.Pair;
                var leftOption = FirstOn(result, exploding, true);
                if (leftOption != null) leftOption.IntValue += ((Number)pair.Left).IntValue;
                var rightOption = FirstOn(result, exploding, false);
                if (rightOption != null) rightOption.IntValue += ((Number)pair.Right).IntValue;
                // Then, the entire exploding pair is replaced with the regular number 0.
                result = (Pair)Replace(result, exploding, MakeO(0));
                continue;
            }

            // the leftmost such regular number splits
            Number ? value = LeftMost10OrAbove(result);
            if (value != null)
            {
                int intValue = value.IntValue;
                result = (Pair)Replace(result, value, Split(intValue));
                continue;
            }
            else break;
        } 
        return result;
    }

    [TestCase("[[[[[9,8],1],2],3],4]", "[[[[0,9],2],3],4]")]
    [TestCase("[7,[6,[5,[4,[3,2]]]]]", "[7,[6,[5,[7,0]]]]")]
    [TestCase("[[6,[5,[4,[3,2]]]],1]", "[[6,[5,[7,0]]],3]")]
    [TestCase("[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]", "[[3,[2,[8,0]]],[9,[5,[7,0]]]]")]
    [TestCase("[[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]", "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]")]
    [TestCase("[[[[0,7],4],[[7,8],[0,[6,7]]]],[1,1]]", "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]")]
    public void TestReduce(string start, string end) => ToString( ReduceSnailfish( ParsePair( start ) ) ).Should().Be( end ); 

    static Pair SumPairs( Pair left, Pair right ) => ReduceSnailfish( Make( left, right ) );

    [TestCase("Day18/input.txt")]
    // [TestCase("Day18/sample.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var values = lines.Select( ParsePair ).ToArray();
        var result = values.Skip(1).Aggregate( values.First(), SumPairs );
        Magnitude(result).Should().Be(4184, "answer 1");

        var indexRange = Enumerable.Range(0, values.Length).ToArray();
        var ii = indexRange.SelectMany(a => indexRange.Select(b => (a, b)).Where(_ => _.Item1 != _.Item2)).ToArray();
        var vv = ii.Select(_ => (ParsePair(lines[_.Item1]), ParsePair(lines[_.Item2]))).ToArray();
        var a2 = vv.Select(_ => Magnitude(SumPairs( _.Item1, _.Item2))).Max();
        a2.Should().Be(4731, "answer 2");
    }
}
