using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using NUnit.Framework;
using System.Text.RegularExpressions;
using System.Diagnostics;

// {}
// []
namespace AdventOfCode2018.Tests9
{

    public class MarbleCircle {
        public readonly LinkedList<int> sequence = new LinkedList<int>();
        public int currentMarbleIndex;
        public Dictionary<int, long> playerKeptMarbles = new Dictionary<int, long>();

        // First, the current player keeps the marble they would have placed, adding it to their score. 
        // In addition, the marble 7 marbles counter-clockwise from the current marble is removed 
        // from the circle and also added to the current player's score. 
        // The marble located immediately clockwise of the marble that was removed becomes the new current marble.
        const int special = 23; 

        public MarbleCircle() {
            // First, the marble numbered 0 is placed in the circle. 
            // At this point, while it contains only a single marble, 
            // it is still a circle: the marble is both clockwise from itself 
            // and counter-clockwise from itself. 
            // This marble is designated the current marble.
            sequence.AddLast(0);
            currentMarbleIndex = 0;              
        }

        bool Turn(int player, int marble) { 
            if ((marble % special) == 0 ) {
                long points = 0;

                // special case
                if ( playerKeptMarbles.ContainsKey(player) ) {
                    points = playerKeptMarbles[player];
                } else {
                    playerKeptMarbles.Add(player,0);
                } 

                points += marble;

                var newIndex = currentMarbleIndex - 7;
                if ( newIndex < 0 )
                {
                    newIndex = sequence.Count + newIndex; // -1 -> last element
                    // Debug.Assert(newIndex < sequence.Count);
                }
                // Debug.Assert(newIndex >= 0); // to be checked what to do

                // var element = sequence[newIndex];
                // sequence.RemoveAt(newIndex);
                var node = GetAt(newIndex);
                var element = node.Value;
                sequence.Remove(element);

                currentMarbleIndex = newIndex;
                // check that new index is still valid after removing
                // Debug.Assert(newIndex < sequence.Count);
                points += element;
                playerKeptMarbles[player] = points;
                return true;
            }
            else
            {
                if ( currentMarbleIndex < sequence.Count - 1 )
                {
                    currentMarbleIndex += 2;
                } 
                else
                {
                    currentMarbleIndex = 1;
                }

                // sequence.Insert(currentMarbleIndex, marble);
                if ( currentMarbleIndex < sequence.Count )
                {
                    var node = GetAt(currentMarbleIndex);
                    sequence.AddBefore(node, marble);
                } 
                else
                {
                    sequence.AddLast(marble);
                }

                return false;
            }
        }

        LinkedListNode<int> GetAt(int index)
        {
            var node = sequence.First;
            for (int j = 0; j < index; j++)
            {
                node = node.Next;
            }
            return node;
        }

        public Tuple<int,long> Winner()
        {
            if (playerKeptMarbles.Any())
            {
                var d = playerKeptMarbles;
                var maxSum = d.Values.Max();
                var winner = d.Single(p => p.Value == maxSum);
                return Tuple.Create(winner.Key, winner.Value);
            }
            throw new Exception("no winner");
        }

        public static Tuple<int, long> Play(int players, int lastMarble)
        {
            var game = new MarbleCircle();
            int currentMarble = 1;
            while (true)
            {
                for (int p = 1; p <= players; p++)
                {
                    game.Turn(p, currentMarble++);
                    if (currentMarble > lastMarble) return game.Winner();
                }
            }
        }
    }

    public class Day9Test
    {
        [TestCase( 9, 25,32)]

        /*
        10 players; last marble is worth 1618 points: high score is 8317
13 players; last marble is worth 7999 points: high score is 146373
17 players; last marble is worth 1104 points: high score is 2764
21 players; last marble is worth 6111 points: high score is 54718
30 players; last marble is worth 5807 points: high score is 37305       
         */
        [TestCase(10, 1618, 8317)]
        [TestCase(13, 7999, 146373)]
        [TestCase(17, 1104, 2764)]
        [TestCase(21, 6111, 54718)]
        [TestCase(30, 5807, 37305)]
        // 425 players; last marble is worth 70848 points
        [TestCase(425, 70848, 413188)] // reply 1
        [TestCase(425, 7084800, 0)] // reply 2 - extremely slow (more then 1 hour?)
        public void TestCase1(int p, int l, int ws)
        {
            var result = MarbleCircle.Play(p, l);
            Assert.AreEqual(ws, result.Item2, "winning score not match");
        }

    }
}
// {}
// []