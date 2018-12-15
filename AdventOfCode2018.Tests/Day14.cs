using System;
using System.Collections.Generic;
using System.Linq;
using NUnit.Framework;

namespace AdventOfCode2018.Day14
{
    public class Day14
    {
        public static LinkedListNode<byte> CiclicMoveNext(LinkedList<byte> list, LinkedListNode<byte> node, int times)
        {
            for ( int i = 0; i < times; i++)
            {
                node = node.Next ?? list.First;
            }
            return node;
        }

        public string Rewind( int rewind, int size=10 )
        {
            var list = new LinkedList<byte>();
            var elf1 = list.AddLast(3);
            var elf2 = list.AddLast(7);
            var count = 2;
            var end = rewind + size;
            while ( count < end)
            {
                var sum = elf1.Value + elf2.Value;
                if ( sum >= 10)
                {
                    // we have 2 new numbers
                    var firstDigit = sum / 10;
                    list.AddLast((byte)firstDigit);
                    var secondDigit = sum - 10;
                    list.AddLast((byte)secondDigit);
                    count += 2;
                } 
                else 
                {
                    // only 1 new number
                    list.AddLast((byte)sum);
                    count++;
                }
                /* After all new recipes are added to the scoreboard, each Elf picks a new current recipe. 
                 * To do this, the Elf steps forward through the scoreboard 
                 * a number of recipes equal to 1 plus the score of their current recipe. 
                 * So, after the first round, the first Elf moves forward 1 + 3 = 4 times, 
                 * while the second Elf moves forward 1 + 7 = 8 times. 
                 * If they run out of recipes, they loop back around to the beginning. 
                 * After the first round, both Elves happen to loop around until they land on the same recipe 
                 * that they had in the beginning; in general, they will move to different recipes.  */
                elf1 = CiclicMoveNext(list, elf1, 1 + elf1.Value);
                elf2 = CiclicMoveNext(list, elf2, 1 + elf2.Value);
            }
            var startSequnce = list.Last; // equal to count
            var curHead = count - 1;
            while (curHead > rewind)
            {
                startSequnce = startSequnce.Previous;
                curHead--;
            }
            var buffer = new System.Text.StringBuilder();
            while (buffer.Length < size)
            {
                buffer.Append(startSequnce.Value);
                startSequnce = startSequnce.Next;
            }
            return buffer.ToString();
        }

        public bool IsSequence(LinkedList<byte> list, LinkedListNode<byte> last, LinkedList<byte> sequence)
        {
            var seqNode = sequence.Last;
            while ( seqNode != null)
            {
                if ( seqNode.Value != last.Value)
                {
                    return false;
                }
                seqNode = seqNode.Previous;
                last = last.Previous;
            }
            return true;
        }

        public long SearchSequence(string sequence)
        {
            var sequenceList = new LinkedList<byte>(sequence.Select(c => (byte)(c - '0')).ToList());
            var list = new LinkedList<byte>();
            var elf1 = list.AddLast(3);
            var elf2 = list.AddLast(7);
            var count = 2;
            while (true)
            {
                var sum = elf1.Value + elf2.Value;
                if (sum >= 10)
                {
                    // we have 2 new numbers
                    var firstDigit = sum / 10;
                    var lastButOne = list.AddLast((byte)firstDigit);
                    var secondDigit = sum - 10;
                    list.AddLast((byte)secondDigit);
                    count += 2;

                    if ( IsSequence(list,lastButOne,sequenceList))
                    {
                        return count - 1 - sequence.Length;
                    }
                }
                else
                {
                    // only 1 new number
                    list.AddLast((byte)sum);
                    count++;
                }

                if (IsSequence(list, list.Last, sequenceList))
                {
                    return count - sequence.Length;
                }

                /* After all new recipes are added to the scoreboard, each Elf picks a new current recipe. 
                 * To do this, the Elf steps forward through the scoreboard 
                 * a number of recipes equal to 1 plus the score of their current recipe. 
                 * So, after the first round, the first Elf moves forward 1 + 3 = 4 times, 
                 * while the second Elf moves forward 1 + 7 = 8 times. 
                 * If they run out of recipes, they loop back around to the beginning. 
                 * After the first round, both Elves happen to loop around until they land on the same recipe 
                 * that they had in the beginning; in general, they will move to different recipes.  */
                elf1 = CiclicMoveNext(list, elf1, 1 + elf1.Value);
                elf2 = CiclicMoveNext(list, elf2, 1 + elf2.Value);
            }
        }

        [Ignore("done")]
        [TestCase(9, "5158916779")]
        [TestCase(5, "0124515891")]

        [TestCase(18, "9251071085")]
        [TestCase(2018, "5941429882")]
        // [TestCase(919901, "0124515891")]

        public void Test1(int rewind, string expected)
        {
            Assert.AreEqual(expected, Rewind(rewind));
        }

        /*
        51589 first appears after 9 recipes.
01245 first appears after 5 recipes.
92510 first appears after 18 recipes.
59414 first appears after 2018 recipes.
        */
        [Ignore("done")]
        [TestCase("51589",9)]
        [TestCase("01245", 5)]
        [TestCase("92510", 18)]
        [TestCase("59414", 2018)]
        [TestCase("919901", 20203532)]
        public void Test2(string sequence, long expected)
        {
            Assert.AreEqual(expected, SearchSequence(sequence));
        }
}
}
