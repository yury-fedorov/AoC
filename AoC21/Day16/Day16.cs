namespace AoC21;

public class Day16Test
{
    const int TypeLiteral = 4;
    class AbstractPackage {
        public int Version;
        public int TypeId;
        public int Length;
    }
    class LiteralPackage : AbstractPackage {
        public List<char> Value = new List<char>();
    }
    class OperatorPackage : AbstractPackage
    {
        public List<AbstractPackage> SubPackages = new List<AbstractPackage>();
    }
    static IEnumerable<char> FromHexToBinaryChar( char hex ) {
        var decimalValue = Convert.ToInt32( $"{hex}", 16);
        decimalValue += 0xF000; // to make it always having last 4 bits after conversion
        var binaryValue = Convert.ToString(decimalValue, 2);
        var l = binaryValue.Length; // ie. 16 but we need 4 -> skip = 16 - 4
        return binaryValue.Skip( l - 4 ).ToArray();
    }

    [TestCase('F', "1111")]
    [TestCase('A', "1010")]
    public void TestHexChar(char hex, string binary) 
        => string.Concat( FromHexToBinaryChar(hex) ).Should().Be(binary);

    public static string FromHexToBinaryString( string hex ) 
        => string.Concat( hex.Select( FromHexToBinaryChar ).SelectMany( _ => _ ) );

    [TestCase("D2FE28", "110100101111111000101000")]
    public void TestHexString(string hex, string binary) => FromHexToBinaryString(hex).Should().Be(binary);

    static int FromBinary(string binary) => Convert.ToInt32(binary, 2);

    static AbstractPackage ReadVersionType( string input, int offset ) {
        var binVersion = input.Substring(offset,3);
        var binType = input.Substring(offset+3, 3);
        return new AbstractPackage() { Version = FromBinary(binVersion), TypeId = FromBinary(binType) };
    }

    static bool IsLiteral(AbstractPackage header) => header.TypeId == TypeLiteral;

    static AbstractPackage ReadPackage(string input, int offset)
    {
        var header = ReadVersionType(input, offset);
        if (IsLiteral(header)) return ReadLiteralPackage(input, offset);
        // 3 - version, 3 - type id, 1 - length type id, 15/11 - length, body
        var lengthTypeIdOffset = offset + 7;
        var lengthTypeId = input[offset + 6];
        // If the length type ID is 0, then the next 15 bits are a number that represents the total length in bits of the sub-packets contained by this packet.
        var isTotalLength = lengthTypeId == '0';
        var nextFieldLength = isTotalLength ? 15 : 11;
        var originalLength = FromBinary(input.Substring(lengthTypeIdOffset, nextFieldLength));
        var length = originalLength;
        var subpackages = new List<AbstractPackage>();
        var bodyOffset = lengthTypeIdOffset + nextFieldLength;
        var nextOffset = bodyOffset;
        if ( isTotalLength )
        {
            // read packages till the total length is ok
            while ( length > 0 )
            {
                var next = ReadPackage(input, nextOffset);
                length -= next.Length;
                nextOffset += next.Length;
                subpackages.Add(next);
            }
        } 
        else
        {
            // number of packages to read
            for ( ; length > 0; length-- )
            {
                var next = ReadPackage(input, nextOffset);
                subpackages.Add(next);
                nextOffset += next.Length;
            }
        }
        var headerOffset = bodyOffset - offset;
        return new OperatorPackage() { Version = header.Version, TypeId = header.TypeId, SubPackages = subpackages, 
            Length = isTotalLength ? ( headerOffset + originalLength ) : ( headerOffset + subpackages.Select( _ => _.Length ).Sum() ) };
    }

    static LiteralPackage ReadLiteralPackage( string input, int offset )
    {
        var header = ReadVersionType(input, offset);
        var value = ReadLiteralValue(input, offset);
        return new LiteralPackage () { Version = header.Version, TypeId = header.TypeId, Value = value, Length = 6 + ( value.Count / 4 * 5 ) };
    }

    public static List<char> ReadLiteralValue( string input, int offset )
    {
        const int BodyOffset = 3 + 3; // Version, TypeId
        var result = new List<char>();
        offset += BodyOffset;
        var isLastGroup = false;
        while ( !isLastGroup )
        {
            var five = input.Substring(offset, 5); // length of the chunk
            isLastGroup = five.First() == '0';
            result.AddRange( five.Substring(1) );
            offset += 5;
        }
        return result;
    }

    [TestCase("D2FE28", 6, 4)]
    public void TestVersionType(string hex, int version, int type)
    {
        var header = ReadVersionType(FromHexToBinaryString(hex), 0);
        header.Version.Should().Be(version);
        header.TypeId.Should().Be(type);
    }

    static int SumVersion( AbstractPackage package )
    {
        if ( package is OperatorPackage operatorPackage )
        {
            return package.Version + operatorPackage.SubPackages.Select(SumVersion).Sum();
        }
        return package.Version;
    }

    [TestCase("38006F45291200", 1, 6, 2)]
    [TestCase("EE00D40C823060", 7, 3,3)]
    public void TestParsing( string hex, int version, int type, int subpackages )
    {
        var bin = FromHexToBinaryString(hex);
        var package = ReadPackage(bin, 0);
        package.Version.Should().Be(version);
        package.TypeId.Should().Be(type);
        (package as OperatorPackage).SubPackages.Count.Should().Be(subpackages);
    }

    [TestCase("8A004A801A8002F478", 16)]
    [TestCase("620080001611562C8802118E34", 12)]
    [TestCase("C0015000016115A2E0802F182340", 23)]
    [TestCase("A0016C880162017C3686B18A3D4780", 31)]
    public void TestSum(string hex, int sum)
    {
        var binInput = FromHexToBinaryString(hex);
        var package = ReadPackage(binInput, 0);
        SumVersion(package).Should().Be(sum);
    }

    [TestCase("Day16/input.txt")]
    public async Task Test(string file) {
        var lines = await App.ReadLines(file);
        var hexInput = lines.First();
        var binInput = FromHexToBinaryString(hexInput);

        var package = ReadPackage(binInput, 0);
        SumVersion(package).Should().Be(977, "answer 1"); // 15107 too high
    }
}
