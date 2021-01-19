using System.Collections.Generic;
using System.Text;

namespace AdventOfCode2018.Day22
{
    public class Map : IMap
    {
        readonly int _depth;
        readonly (int, int) _target;
        readonly IDictionary<(int, int), long> _pointIndexMap = new Dictionary<(int, int), long>();
        public Map(int depth, (int, int) target)
        {
            _depth = depth;
            _target = target;
        }

        public int ToErosionLevel(long geologicalIndex) => (int)((geologicalIndex + _depth) % 20183);

        public long GeologicalIndex((int, int) point)
        {
            if (_pointIndexMap.TryGetValue(point, out var value)) return value;

            var (x, y) = point;
            if (point == (0, 0)) value = 0;
            else if (point == _target) value = 0;
            else if (y == 0) value = (x * 16807);
            else if (x == 0) value = (y * 48271);
            else value = ToErosionLevel(GeologicalIndex((x - 1, y))) * ToErosionLevel(GeologicalIndex((x, y - 1)));
            _pointIndexMap.Add(point, value);
            return value;
        }

        public Material ToMaterial(long geologicalIndex) => (Material)(ToErosionLevel(geologicalIndex) % 3);

        public Material At(int x, int y) => x >= 0 && y >= 0 ? ToMaterial(GeologicalIndex((x, y))) : Material.SolidRock;

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
                    var material = At(x, y);
                    char ch = mapping[(int)material];
                    map.Append(ch);
                }
                map.AppendLine();
            }
            return map.ToString();
        }
    }
}
