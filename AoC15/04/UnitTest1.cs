using System;
using Xunit;

namespace _04
{
    public class UnitTest1
    {
        [Fact]
        public void Test1()
        {
            const string start = "yzbqklnj";

            using var md5 = System.Security.Cryptography.MD5.Create();
            for ( int i = 0; true; i++ )
            {
                var input = $"{start}{i}";
                byte[] inputBytes = System.Text.Encoding.ASCII.GetBytes(input);
                byte[] hashBytes = md5.ComputeHash(inputBytes);
                var hex = BitConverter.ToString(hashBytes).Replace("-", string.Empty);
                Assert.False(hex.StartsWith("000000"), $"Found {i} - {hex}");
            }
        }
    }
}
