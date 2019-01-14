using NUnit.Framework;

namespace AdventOfCode2018
{
    public class Day19Task2
    {
        [TestCase(17540352)]
        public void Test(int er0)
        {
            // Assert.AreEqual(er0, Calculate(r0));
            Assert.AreEqual(er0, CalculateFaster());
        }

        int CalculateFaster()
        {
            const int r4 = 10551387;
            int r0 = 0;
            for (int r1 = 1; r1 <= r4; r1++)
            {
                if ( r4 % r1 == 0 ) {
                    r0 += r1; // 7   addr 1 0 0
                }                    
            }
            return r0;
        }

        /*
        int Calculate()
        {
            const int r4 = 10551387;
            int r0 = 0;
            for ( int r1 = 1; r1 < r4; r1++ )
            {
                for (int r3 = 1; r3 < r4; r3++)
                {
                    var r5 = r1 * r3; //  // 3   mulr 1 3 5
                    if ( r5 == r4 )
                    {
                        r0 += r1; // 7   addr 1 0 0
                    }
                }
            }
            return r0;
        }

	    // https://adventofcode.com/2018/day/16
        // https://adventofcode.com/2018/day/19
        int Calculate( int r0 )
        {
            int r1, r3, r4, r5;
            r1 = r3 = r4 = r5 = 0;

            // 0   addi 2 16 2
            // r[2] += 16;
            goto l17;
        
            // 1   seti 1 0 1
            l01:
            r1 = 1;

            // 2   seti 1 3 3
            l02:
            r3 = 1;

            // 3   mulr 1 3 5
            l03:
            r5 = r1 * r3;

            // 4   eqrr 5 4 5
            r5 = ( r4 == r5 ) ? 1 : 0;

            // 5   addr 5 2 2
        
            if (r5 != 1)
            {
                // 6   addi 2 1 2
                goto l08;
            }

            // 7   addr 1 0 0
            r0 += r1;

            // 8   addi 3 1 3
            l08:
            r3 += 1;

            // 9   gtrr 3 4 5
            r5 = ( r3 > r4 ) ? 1 : 0;

            // 10  addr 2 5 2
        
            if ( r5 != 1 )
            {
                // 11  seti 2 6 2
                goto l03;
            }

            // 12  addi 1 1 1
            r1 += 1;

            // 13  gtrr 1 4 5
            r5 = r1 > r4 ? 1 : 0;

            // 14  addr 5 2 2
        
            if ( r5 != 1 )
            {
                // 15  seti 1 1 2
                goto l02;
            }

            // 16  mulr 2 2 2
            return r0; // the end (goto to 16*16)

            // 17  addi 4 2 4
            l17:
            r4 += 2;

            // 18  mulr 4 4 4
            r4 *= r4;

            // 19  mulr 2 4 4
            r4 *= 19;

            // 20  muli 4 11 4
            r4 *= 11;

            // 21  addi 5 6 5
            r5 += 6;
            // 22  mulr 5 2 5
            r5 *= 22;

            // 23  addi 5 19 5
            r5 += 19;

            // 24  addr 4 5 4
            r4 += r5;

            // 25  addr 2 0 2
            // Debug.Assert(false);
            //r2 += r0; // what is in register 0???
            // TODO goto r[2];
            return r0;

            // 26  seti 0 7 2
            goto l01;

            // 27  setr 2 6 5
            // r[5] = r[2];
            r5 = 27;

            // 28  mulr 5 2 5
            // r[5] *= r[2];
            r5 *= 28;

            // 29  addr 2 5 5
            // r[5] += r[2];
            r5 += 29;

            // 30  mulr 2 5 5
            r5 *= 30;

            // 31  muli 5 14 5
            r5 *= 14;

            // 32  mulr 5 2 5
            r5 *= 32;

            // 33  addr 4 5 4
            r4 += r5;

            // 34  seti 0 7 0
            r0 = 0;

            // 35  seti 0 3 2
            goto l01;
        }
        */
    }
}
