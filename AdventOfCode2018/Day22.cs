using NUnit.Framework;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace AdventOfCode2018
{
    public enum Material { Rocky, Wet, Narrow }

    public class Map
    {
        readonly int _depth;
        readonly (int, int) _target;
        readonly IDictionary<(int, int), BigInteger> _pointIndexMap = new Dictionary<(int, int), BigInteger>();
        public Map(int depth, (int,int) target)
        {
            _depth = depth;
            _target = target;
        }

        public int ToErosionLevel(BigInteger geologicalIndex) => (int)((geologicalIndex + _depth) % 20183);

        public BigInteger GeologicalIndex( (int,int) point )
        {
            BigInteger value;
            if (_pointIndexMap.TryGetValue(point, out value)) return value;

            var (x, y) = point;
            if (point == (0, 0) ) value = 0;
            else if (point == _target) value = 0;
            else if (y == 0) value = (x * 16807);
            else if (x == 0) value = (y * 48271);
            else value = ToErosionLevel(GeologicalIndex( (x-1, y) )) * ToErosionLevel(GeologicalIndex( (x, y-1) ));
            _pointIndexMap.Add(point, value);
            return value;
        }

        public Material ToMaterial(BigInteger geologicalIndex) => (Material)( ToErosionLevel(geologicalIndex) % 3 );

        public Material At((int, int) point) => ToMaterial(GeologicalIndex(point));

        public string Draw()
        {
            // Rocky, Wet, Narrow - Drawing this same cave system with rocky as ., wet as =, narrow as |
            char[] mapping = { '.', '=', '|' }; 
            var (tx, ty) = _target;
            var map = new StringBuilder();
            for (var y = 0; y <= ty; y++)
            {
                for (var x = 0; x <= tx; x++)
                {
                    var material = At((x, y));
                    char ch = mapping[(int)material];
                    map.Append(ch);
                }
                map.AppendLine();
            }
            return map.ToString();
        }
    }

    public class Day22
    {
        // Puzzle input
        // depth: 7863
        // target: 14,760
        [TestCase(510,10,10,114)]
        [TestCase(7863, 14, 760, 11462)] // 786 - too low
        public void Test(int depth, int tx, int ty, int eRiskLevel)
        {
            var map = new Map(depth, (tx,ty) );
            var a =  map.At((1, 1));
            int riskLevel = 0;
            for (var x = 0; x <= tx; x++ )
            {
                for (var y = 0; y <= ty; y++)
                {
                    var material = map.At((x, y));
                    riskLevel += (int)material;
                }
            }
            var image = map.Draw();
            Assert.AreEqual(eRiskLevel, riskLevel); 
        }
    }
}
