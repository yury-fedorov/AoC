using System;
using System.IO;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace _12
{
    class Program
    {

        static int ? Count( JsonElement node ) {
            switch ( node.ValueKind ) {
                case JsonValueKind.Array:
                var count = 0;
                var arraySize = node.GetArrayLength();
                for ( var i = 0; i < arraySize; i++ ) {
                    count += Count(node[i]) ?? 0;
                }
                return count;
            case JsonValueKind.Number:
                return node.GetInt32();
            case JsonValueKind.String:
                return node.GetString() == "red" ? null : 0;
            case JsonValueKind.Object:
                var objCount = 0;
                var iter = node.EnumerateObject();
                do {
                    var propCount = Count(iter.Current.Value);
                    if ( !propCount.HasValue ) return 0;
                    objCount += propCount.Value;
                } while ( iter.MoveNext() );
                return objCount;
            case JsonValueKind.Undefined:
                return 0;
            default:
                Console.Error.WriteLine(node.ValueKind);
                Console.Error.WriteLine(node);
                return 0;
            }
        }

        static void Main(string[] args)
        {
            var json = File.ReadAllText("input.txt");
            var tree = JsonSerializer.Deserialize<JsonElement>(json);
            var count = Count(tree);
            Console.WriteLine(count);
        }
    }
}
