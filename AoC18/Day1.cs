using System;
using System.Linq;
using System.Collections.Generic;

namespace AdventOfCode2018
{
    public class Day1 {

        public IEnumerable<int> ParseSequence(string frequencyChanges, char separator) 
            => frequencyChanges.Split(separator).Select( a => Convert.ToInt32(a.Trim()) );
        
        // first task
        public int CalculateFrequency(string frequencyChanges, char separator=',')
            => ParseSequence(frequencyChanges, separator).Sum();

        public int FirstRepetition(string frequencyChanges, char separator){
            var sequance = ParseSequence(frequencyChanges, separator).ToArray();
            var set = new HashSet<int>();
            var sum = 0;              
            while (true) {
                var curSequance = sequance;
                foreach( var delta in curSequance ) {
                    set.Add(sum);
                    sum += delta;
                    if ( set.Contains(sum) )
                        return sum;
                }
            }  
        }
    }
}
