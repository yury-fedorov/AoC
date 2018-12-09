using System;
using AdventOfCode2018.Tests9;

namespace Test
{
    class Program
    {
        static void Main(string[] args)
        {
            var result = MarbleCircle.Play(425, 7084800);
            Console.Out.WriteLine(result.Item2);
        }
    }
}
