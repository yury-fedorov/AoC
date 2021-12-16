using LanguageExt;

namespace AoC21;

public class Day16Test
{
    static IEnumerable<char> FromHexToBinaryChar( char hex ) {
        var decimalValue = Convert.ToInt32( $"{hex}", 16);
        decimalValue += 0xF000; // to make it always having last 4 bits after conversion
        var binaryValue = Convert.ToString(decimalValue, 2);
        var l = binaryValue.Length; // ie. 16 but we need 4 -> skip = 16 - 4
        return binaryValue.Skip( l - 4 ).ToArray();
    }

    [TestCase('F', "1111")]
    [TestCase('A', "1010")]
    public async Task TestHexChar(char hex, string binary) 
        => String.Concat( FromHexToBinaryChar(hex) ).Should().Be(binary);

    public static string FromHexToBinaryString( string hex ) 
        => String.Concat( hex.Select( FromHexToBinaryChar ).SelectMany( _ => _ ) );

    [TestCase("D2FE28", "110100101111111000101000")]
    public async Task TestHexString(string hex, string binary) => FromHexToBinaryString(hex).Should().Be(binary);

    static int FromBinary(string binary) => Convert.ToInt32(binary, 2);
    public static ( int Version, int TypeId ) ReadVersionType( string input, int offset ) {
        var binVersion = input.Substring(offset,3);
        var binType = input.Substring(offset+3, 3);
        return ( FromBinary(binVersion), FromBinary(binType) );
    }

    [TestCase("D2FE28", 6, 4)]
    public async Task TestVersionType(string hex, int version, int type) 
        => ReadVersionType(hex,0).Should().Be( (version, type) );

    [TestCase("Day16/input.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var hexInput = lines.First(); 
        var binInput = FromHexToBinaryString( hexInput );
        binInput.Length.Should().Be(-1, "answer 1");
    }
}