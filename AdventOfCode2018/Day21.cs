using System;
using NUnit.Framework;

namespace AdventOfCode2018
{
    public class Day21
    {
        void CodeV1() 
        {
            long r0, r2, r3, r4, r5;
            r0 = r2 = r3 = r4 = r5 = 0;

            r5 = 123;                   // 00: seti 123 0 5
        l01:
            r5 &= 456;                  // 01: bani 5 456 5
            r5 = (r5 == 72) ? 1 : 0;    // 02: eqri 5 72 5
            if (r5 == 0)                // 03: addr 5 1 1
                                        
                goto l01;               // 04: seti 0 0 1
            // 05: seti 0 4 5
            r5 = 0;
            // bori 5 65536 3
            r3 = r5 | 65536;
            // seti 521363 7 5
            r5 = 521363;
            // bani 3 255 4
            r4 = r3 & 255;
            // addr 5 4 5
            r5 += r4;
            // bani 5 16777215 5
            r5 &= 16777215;
            // muli 5 65899 5
            r5 *= 65899;
            // bani 5 16777215 5
            r5 &= 16777215;
            // gtir 256 3 4
            // addr 4 1 1
            // addi 1 1 1
            // seti 27 1 1
            // seti 0 2 4
            // addi 4 1 2
            // muli 2 256 2
            // gtrr 2 3 2
            // addr 2 1 1
            // addi 1 1 1
            // seti 25 2 1
            // addi 4 1 4
            // seti 17 3 1
            // setr 4 2 3
            // seti 7 1 1
            // eqrr 5 0 4
            // addr 4 1 1
            // seti 5 8 1      
            }

        [TestCase()]
        public void Test()
        {
            Assert.Fail("to be implemented");
        }
    }
}
