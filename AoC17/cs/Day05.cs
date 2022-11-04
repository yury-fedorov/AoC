using System;
using NUnit.Framework;
using System.IO;
using System.Linq;

namespace AoC17
{
    public class Day05
    {
	public int CountSteps(int [] jumpOffsets, Func<int,int> delta )
	{
	    var jo = (int[])jumpOffsets.Clone(); // to avoid changing the array passed as argument
	    int count = 0;
	    int current = 0;
	    for ( ; current < jo.Length; count++ )
	    {
	        var offset = jo[current];
	        var newCurrent = current + offset;
                jo[current] = offset + delta(offset);
	        current = newCurrent;
	    }
	    Console.WriteLine( string.Join( ' ', jo.Select( n => n.ToString() ).ToArray()) );
	    return count;		
	}
        
        [TestCase("Day05Sample.txt",5, 10)]
	[TestCase("Day05Input.txt",343364, 25071947)]
        public void Test1(string file, int expected1, int expected2)
        {
            var lines = File.ReadAllLines(Path.Combine( App.Directory, file));
            var jumpOffsets = lines.Select( l => Convert.ToInt32(l) ).ToArray();
            Assert.AreEqual(expected1, CountSteps(jumpOffsets, (offset)=>1 ));
            Assert.AreEqual(expected2, CountSteps(jumpOffsets, (offset)=> ( offset < 3 ? 1 : -1 ) ));
        }
    }
}
