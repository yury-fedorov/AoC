using System;
using System.Linq;
using System.Collections.Generic;

namespace AdventOfCode2018
{
    public class Day2 {
        public IDictionary<char,int> GroupLetters(string boxId) {
            return boxId.GroupBy(c => c) // put each character into a "bucket"
                     // order the buckets alphabetically
                     .OrderBy(c => c.Key)
                     // then convert to dictionary where key = character, value = count
                     .ToDictionary(grp => grp.Key, grp => grp.Count());
        }

        public int CountOccurancies(IDictionary<char,int> letterToCount, int filter)
            => letterToCount.Where(a => a.Value == filter).Count();

        public Tuple<int,int> CalculateRudimentaryChecksum(string boxId) {
            var dictionary = GroupLetters(boxId);            
            return Tuple.Create(
                CountOccurancies(dictionary, 2) > 0 ? 1 : 0, 
                CountOccurancies(dictionary, 3) > 0 ? 1 : 0 );
        }

        // Task 2
        // this is found by removing the differing character from either ID, 
        public string IdentifyCorrectBoxId(string a, string b) {
            if (a.Length != b.Length) return null;
            int ? wrongIndex = null;
            for ( int i = 0; i < a.Length; i++ ) {
                if ( a[i] != b[i] ) {
                    // we have found a difference
                    if (wrongIndex.HasValue) return null; // more then one difference
                    wrongIndex = i;
                }
            }
            if (!wrongIndex.HasValue) return null; // perfect match is not good as well
            var list = a.ToList();
            list.RemoveAt(wrongIndex.Value);
            return String.Concat(list);
        }

        public string IdentifyCorrectBoxId(IList<string> idList) {
            var set = new HashSet<string>();
            for(int i = 0; i < idList.Count; i++) {
                for(int j = 0; j < i; j++ ) {
                    var correctId = IdentifyCorrectBoxId(idList[i],idList[j]);
                    if (correctId != null) set.Add(correctId);
                }
            }
            return set.Single();
        }
    }
}