using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;

namespace AdventOfCode2018.Tests8
{
    class Node {
        // header
        public int countChildNodes;
        public int countMetaData;
        public List<Node> childNodes; // Zero or more child nodes (as specified in the header)
        public List<int> metadata; // One or more metadata entries (as specified in the header)
        public List<int> rawData; // ie. "0 1 99"

        public int Length()
        {
            return 2 + countMetaData + childNodes.Select(n => n.Length()).Sum(); 
        }
    }

    public class Day8Test
    {
        Node Read(List<int> tree)
        {
            var node = new Node { 
                countChildNodes = tree[0],
                countMetaData = tree[1],
                rawData = tree };
            Debug.Assert(node.countMetaData > 0);
            node.metadata = tree.GetRange(
                tree.Count - node.countMetaData,
                node.countMetaData);
            node.childNodes = new List<Node>();

            // the most difficult is to split the nodes
            if (node.countChildNodes > 0)
            {
                var allKinds = tree.GetRange(2, tree.Count - 2 - node.countMetaData);
                node.childNodes.AddRange(ReadKids(node.countChildNodes, allKinds));
            }
            return node;
        }

        // bounds here are precise
        IEnumerable<Node> ReadKids(int missingChildren, List<int> allChildren)
        {
            Debug.Assert(missingChildren > 0);
            if (missingChildren == 1)
            {
                // it is the last node
                return new[] { Read(allChildren) };
            }
            // we are here when we have at least 2 nodes
            var nextChildNodes = allChildren[0]; 
            var nextMetaData = allChildren[1];

            if (nextChildNodes == 0)
            {
                // the length is fixed
                var nodeLength = 2 + nextMetaData;
                var child = Read(allChildren.GetRange(0, nodeLength));
                var newKids = allChildren.GetRange(nodeLength, allChildren.Count - nodeLength);
                return ( new[] { child } ).Concat( ReadKids(missingChildren-1,newKids) );
            }

            // we are hear when we cannot parse current level, go deeper
            var deepNode = ReadDeepNode(allChildren); // end is not precise

            // length of node
            var length = deepNode.Length();
            var newKidsAfterDeep = allChildren.GetRange(length, allChildren.Count - length);
            return (new[] { deepNode }).Concat(ReadKids(missingChildren - 1, newKidsAfterDeep));
        }

        Node ReadDeepNode(List<int> uncertainKids ) // end is not precise
        {
            var nextChildNodes = uncertainKids[0];
            var nextMetaData = uncertainKids[1];
            if (nextChildNodes == 0)
            {
                // the length is fixed
                var nodeLength = 2 + nextMetaData;
                return Read(uncertainKids.GetRange(0, nodeLength));
            }
            // one node approach cannot be used, the end is not precise

            var deepNode = new Node
            {
                countChildNodes = nextChildNodes,
                countMetaData = nextMetaData
            };
            Debug.Assert(deepNode.countMetaData > 0);
            deepNode.childNodes = new List<Node>();
            while( deepNode.childNodes.Count < deepNode.countChildNodes )
            {
                var shift = 2 + deepNode.childNodes.Select(c => c.Length()).Sum();
                var child = ReadDeepNode(uncertainKids.GetRange(shift, uncertainKids.Count - shift - nextMetaData));
                var length = child.Length();
                deepNode.childNodes.Add(child);
            }
            var shiftMeta = 2 + deepNode.childNodes.Select(c => c.Length()).Sum();
            deepNode.metadata = uncertainKids.GetRange(shiftMeta, nextMetaData);
            return deepNode;
        }

        // sum of all metadata entries of the tree
        int t1(Node node )
        {
            return node.metadata.Sum()
                + node.childNodes.Select(a => t1(a)).Sum();
        }


        int t2(Node node) {
            return node.countChildNodes == 0 ? t1(node) :
                node.metadata.Select(i => i - 1)
                    .Where(i => i >= 0 && i < node.countChildNodes)
                    .Select(i => node.childNodes.ElementAt(i))
                    .Select(n => t2(n))
                    .Sum();
        }

        [TestCase("Day8Sample.txt", 138, 66)]
        [TestCase("Day8Input.txt", 42472, 21810)]
        public void TestCase1(string file, int esm1,int er2) {
            var lines = File.ReadAllLines(Path.Combine(App.Directory, file));
            var tree = Read( lines.Single().Split(" ").Select(a=>Convert.ToInt32(a)).ToList() );
            var r1 = t1(tree);
            Assert.AreEqual(esm1,r1, "wrong result 1");

            var r2 = t2(tree);
            Assert.AreEqual(er2, r2, "wrong result 2");
                      
        }
    }
}