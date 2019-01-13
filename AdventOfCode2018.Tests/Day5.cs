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
                var list = new LinkedList<char>(polimer);
                var node = list.First;
                while ( node.Next != null )
                {
                    char b = node.Value;
                    char a = node.Next.Value;
                    if (IsOpposite(a, b))
                    {
                        // we destroy them
                        var newNode = node.Previous ?? node.Next.Next;
                        if (newNode == null) return string.Empty;
                        list.Remove(node.Next); 
                        list.Remove(node);
                        node = newNode;
                    } else node = node.Next;
                }
                return string.Concat(list);
            }
        }
    }
}
