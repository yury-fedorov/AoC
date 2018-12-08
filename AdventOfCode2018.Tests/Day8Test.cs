using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using NUnit.Framework;
using System.Text.RegularExpressions;
using System.Diagnostics;

// {}
// []
namespace AdventOfCode2018.Tests8
{
    

    public class Day8Test
    {
        // sum of all metadata entries of the tree
        int t1(string line) { return 0; }

        [TestCase("Day8Sample.txt", 138)]
        [TestCase("Day8Input.txt", 0)]
        public void TestCase1(string file, int esm1) {
            var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file));
            var line = lines.Single();
            var r1 = t1(line);
            Assert.AreEqual(esm1,r1, "wrong result 1");
            /*
            var r2 = t2(lines, deltaDuration, workers);
            Assert.AreEqual(ed, r2, "wrong result 2");
            */           
        }
    }
}
// {}
// []