namespace AoC21;

public class Day03Test
{
    static int CO2(List<string> values, bool isMost)
    {
        var parityValue = isMost ? '1' : '0';
        for (int i = 0; values.Count() != 1; i++)
        {
            var half = (int)Math.Ceiling(values.Count() / 2.0);
            bool isEven = values.Count() % 2 == 0;
            var oneCount = values.Count(l => l[i] == '1');

            char ch = (isEven && oneCount == half)
                ? parityValue // parity case
                : (oneCount >= half) ^ (!isMost) ? '1' : '0';

            values = values.Where(l => l[i] == ch).ToList();
        }
        return Convert.ToInt32(values.First(), 2); // binary to decimal
    }

    [TestCase("Day03/input.txt")]
    public async Task Test(string file)
    {
        var lines = await App.ReadLines(file);
        var values = lines.ToList();
        var n = values.First().Length;
        var half = (int)Math.Ceiling(values.Count() / 2.0);
        int gamma = 0;
        for (int i = 0; i < n; i++)
        {
            var oneCount = values.Count(l => l[i] == '1');
            gamma <<= 1;
            gamma |= (oneCount >= half ? 1 : 0);
        }
        var mask = (1 << n) - 1;  // setting all significant bits to 1
        var epsylon = (gamma ^ mask) & mask; // inversion of significant bits (010)->(101)
        (gamma * epsylon).Should().Be(3959450, "answer 1");

        var o = CO2(values, true);
        var s = CO2(values, false);
        (o * s).Should().Be(7440311, "answer 2");
    }
}