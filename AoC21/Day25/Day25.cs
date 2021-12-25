namespace AoC21;

public class Day25Test
{
    record Point( int X, int Y ) {}
    enum Hord { East = '>', South = 'v' }

    [TestCase("Day25/input.txt")]
    // [TestCase("Day25/sample.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var arrayMap = lines.Select( _ => _.ToCharArray() ).ToArray();
        var xsize = arrayMap.First().Length();
        var ysize = arrayMap.Length();
        Func<char,bool> isHord = (ch) => ch switch { (char)Hord.East or (char)Hord.South => true, _  => false };
        var map = Enumerable.Range( 0, xsize ).SelectMany( 
            x => Enumerable.Range( 0, ysize ).Select( y => ( new Point (x,y), arrayMap[y][x] ) ) )
            .Where( _ => isHord( _.Item2 ) ).ToDictionary( _ => _.Item1, _ => (Hord)_.Item2 );

        Func<Point,Point> down = (p) => new Point( p.X, ( p.Y + 1 ) % ysize );
        Func<Point,Point> left = (p) => new Point( ( p.X + 1 ) % xsize, p.Y );

        var hordOrder = new [] { Hord.East, Hord.South };

        int step = 0;
        while(true) {
            bool isMoved = false;
            
            foreach ( var hord in hordOrder ) {
                var map1 = new Dictionary<Point,Hord>();
                var ahead = hord == Hord.East ? left : down;
                foreach ( var c in map.Where( _ => _.Value == hord ) ) {
                    var p = c.Key;
                    var p1 = ahead(p);
                    var isFreeSpace = !map.ContainsKey( p1 );
                    map1.Add( isFreeSpace ? p1 : p, hord );
                    isMoved |= isFreeSpace;
                }
                map.Where( _ => _.Value != hord ).ToList().ForEach(_ => map1.Add( _.Key, _.Value ) );
                map = map1;
            }
            step++;
            if (!isMoved) break;
        }

        step.Should().Be(598, "answer 1");
    }
}