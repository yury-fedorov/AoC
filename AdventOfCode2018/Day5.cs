using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text.RegularExpressions;

// {} []
namespace AdventOfCode2018
{
    namespace Day5
    {
        public class Day5
        {
            public bool IsOpposite(char a, char b) {
                return a != b && Char.ToLower(a) == Char.ToLower(b);
            }

            public string OptimizePolimer(string polimer) 
            {
                var list = polimer.ToList();
                for ( int i = 1; i < list.Count ; i++) {
                    char b = list[i];
                    char a = list[i - 1];
                    if (IsOpposite(a, b))
                    {
                        // we destroy them
                        list.RemoveAt(i);
                        list.RemoveAt(i - 1);
                    }
                }
                
                var shorter = string.Concat(list);
                return polimer.Length > shorter.Length ? OptimizePolimer(shorter) : polimer;
            }
        }
    }
}
