using System;
using Xunit;

namespace AoC15
{
    public class Day04
    {
        private int Answer(string start, string pattern)
        {
            using var md5 = System.Security.Cryptography.MD5.Create();
            for (int result = 0; true; result++)
            {
                var input = $"{start}{result}";
                byte[] inputBytes = System.Text.Encoding.ASCII.GetBytes(input);
                byte[] hashBytes = md5.ComputeHash(inputBytes);
                var hex = BitConverter.ToString(hashBytes).Replace("-", string.Empty);
                if (hex.StartsWith(pattern))
                    return result;
            }
        }

        [Fact]
        public void Solution()
        {
            const string start = "yzbqklnj";
            Assert.Equal(282749, Answer(start,"00000"));
            Assert.Equal(9962624, Answer(start, "000000"));
        }
    }
}
