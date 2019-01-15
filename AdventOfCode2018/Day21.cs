using System;
using NUnit.Framework;

namespace AdventOfCode2018
{
    public class Day21
    {
        void CodeV1(long r0) 
        {
            long r2, r3, r4, r5;
            r2 = r3 = r4 = r5 = 0;

/* dummy logic
            r5 = 123;       // 7B       // 00: seti 123 0 5
       l01: r5 &= 456;      // 1C8      // 01: bani 5 456 5
		            // 48
            r5 = (r5 == 72) ? 1 : 0;    // 02: eqri 5 72 5
            if (r5 == 0)                // 03: addr 5 1 1                                        
                goto l01;               // 04: seti 0 0 1            
*/
            r5 = 0;            		// 05: seti 0 4 5
       l06: r3 = r5 | 65536; // 10000	// 06: bori 5 65536 3            
            r5 = 521363;     // 7F493	// 07: seti 521363 7 5            
       l08: r4 = r3 & 255;   // FF      // 08: bani 3 255 4
            r5 += r4;            	// 09: addr 5 4 5
            r5 &= 16777215;            	// 10: bani 5 16777215 5
            r5 *= 65899;    // 1016B    // 11: muli 5 65899 5
            r5 &= 16777215; // FFFFFF	// 12: bani 5 16777215 5
	    r4 = 256 > r3 ? 1 : 0;	// 13: gtir 256 3 4
            if (r4 == 0) 		// 14: addr 4 1 1
            	goto l17; 		// 15: addi 1 1 1            
	    goto l28; 			// 16: seti 27 1 1            
       l17: r4 = 0; 			// 17: seti 0 2 4
       l18: r2 = r4 + 1; 		// 18: addi 4 1 2
	    r2 *= 256;			// 19: muli 2 256 2            
	    r2 = r2 > r3 ? 1 : 0;	// 20: gtrr 2 3 2
            if ( r2 == 0 )              // 21: addr 2 1 1
               goto l24;		// 22: addi 1 1 1 
	    goto l26;			// 23: seti 25 2 1
       l24: r4++; 			// 24: addi 4 1 4
            goto l18;			// 25: seti 17 3 1
       l26: r3 = r4; 			// 26: setr 4 2 3
            goto l08;			// 27: seti 7 1 1
       l28: if (r5 == r0) 		// 28: eqrr 5 0 4
            return;			// 29: addr 4 1 1
            goto l06; 			// 30: seti 5 8 1      
        }

        [TestCase()]
        public void Test()
        {
            Assert.Fail("to be implemented");
        }
    }
}
