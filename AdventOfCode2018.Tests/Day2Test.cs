using NUnit.Framework;
using System.IO;
using System;
using System.Linq;

namespace AdventOfCode2018
{
    public class Day2Test
    {

        private readonly Day2 day2 = new Day2();

        [TestCase("abcdef",0,0)] // contains no letters that appear exactly two or three times.
        [TestCase("bababc",1,1)] // contains two a and three b, so it counts for both.
        [TestCase("abbcde",1,0)] // contains two b, but no letter appears exactly three times.
        [TestCase("abcccd",0,1)] // contains three c, but no letter appears exactly two times.
        [TestCase("aabcdd",2,0)] // contains two a and two d, but it only counts once.
        [TestCase("abcdee",1,0)] // contains two e.
        [TestCase("ababab",0,2)] // contains three a and three b, but it only counts once.
        public void CheckOccurancies(string boxId, int expectedTwoTimes, int expectedThreeTimes) {            
            var result = day2.CalculateRudimentaryChecksum(boxId);
            Assert.AreEqual( Tuple.Create(expectedTwoTimes>0?1:0,expectedThreeTimes>0?1:0), result, "Wrong checksum");
            var dictionary = day2.GroupLetters(boxId);
            Assert.AreEqual(expectedTwoTimes,day2.CountOccurancies(dictionary,2), "Wrong 2 times count" );            
            Assert.AreEqual(expectedThreeTimes,day2.CountOccurancies(dictionary,3), "Wrong 3 times count" );            
        }

        [TestCase("Day2ExampleInput.txt",12)]
        [TestCase("Day2Input.txt",5658)]
        public void Test3(string file, int expected) {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory,file));
            var result = lines.Select(line => day2.CalculateRudimentaryChecksum(line))
                .Aggregate(Tuple.Create(0,0), (a,b) => Tuple.Create(a.Item1+b.Item1,a.Item2+b.Item2) );
            var checksum = result.Item1 * result.Item2;
            Assert.AreEqual(expected,checksum,"Wrong checksum");
        }

        [TestCase("fghij","fguij","fgij")]
        public void TestCommon(string a, string b, string ab) {
            var ab1 = day2.IdentifyCorrectBoxId(a,b);
            Assert.AreEqual(ab,ab1,"Wrong correct box ID");
        }

        [TestCase("Day2Input.txt", "nmgyjkpruszlbaqwficavxneo")]
        public void Test4(string file, string commonBoxId) {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory,file));            
            Assert.AreEqual(commonBoxId, day2.IdentifyCorrectBoxId(lines), "Final result");
        }
    }
}