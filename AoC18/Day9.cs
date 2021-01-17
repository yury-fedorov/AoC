using System;
using System.Collections.Generic;
using System.Linq;
using NUnit.Framework;

namespace AdventOfCode2018.Tests9 {

    public class MarbleCircle {
        public readonly LinkedList<int> sequence = new LinkedList<int>();
        public LinkedListNode<int> currentMarble;
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
            currentMarble = sequence.AddLast(0);             
        }

        void Turn(int player, int marble) { 
            if ((marble % special) == 0 ) {
                long points;

                // special case
                if ( playerKeptMarbles.ContainsKey(player) ) {
                    points = playerKeptMarbles[player];
                } else {
                    playerKeptMarbles.Add(player,points = 0);
                } 

                points += marble;
                for ( int j = 0; j < 7; j++)
                {
                    currentMarble = currentMarble.Previous ?? sequence.Last;
                }

                var nodeToRemove = currentMarble;
                currentMarble = currentMarble.Next ?? sequence.First;
                sequence.Remove(nodeToRemove);

                points += nodeToRemove.Value;
                playerKeptMarbles[player] = points;
            }
            else
            {
                var node = currentMarble.Next ?? sequence.First;
                currentMarble = sequence.AddAfter(node, marble);
            }
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
            var player = 1;
            for (int currentMarble = 1; currentMarble <= lastMarble; currentMarble++ )
            {
                game.Turn(player, currentMarble);
                player = player < players ? player + 1 : 1;
            }
            return game.Winner();
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
        [TestCase(425, 70848, 413188)] // task1
        public void Day9Solutions(int p, int l, long ws)
        {
            var result = MarbleCircle.Play(p, l);
            Assert.AreEqual(ws, result.Item2, "winning score not match");
        }

        [Ignore("extremely slow (more then 1 hour?")]
        [TestCase(425, 7084800, 3377272893)] // task 2
        public void Day9Task2(int p, int l, long ws)
            => Day9Solutions(p, l, ws);
    }
}