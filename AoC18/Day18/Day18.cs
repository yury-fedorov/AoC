﻿using NUnit.Framework;
using NUnit.Framework.Legacy;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace AdventOfCode2018.Day18
{
	public class Day18
	{
		// ground (.), trees (|), or a lumberyard (#)
		public const char Ground = '.';
		public const char Tree = '|';
		public const char Lumberyard = '#';

        public string ToHash(char[,]map, (int X, int Y) size)
        {
            var s = new StringBuilder();
            for (int y = 0; y < size.Y; y++)
            {
                for (int x = 0; x < size.X; x++)
                    s.Append(map[x, y]);
                s.AppendLine();
            }
            return s.ToString();
        }

		public static long ToAnswer(int wood, int lumber) => wood * lumber;

		const string FileName = "Day18/input.txt";

		[TestCase(FileName,10, 514944)] // answer 1
        [TestCase("Day18/sample.txt",10, 1147)]
		[TestCase(FileName, 1000000000, 193050)] // answer 2 -> // Identified loop size: 28 on time: 634
		public void Solution(string file, long time, long expectedAnswer )
		{
			var lines = File.ReadAllLines(Path.Combine(App.Directory, file));
			var size = (lines[0].Length, lines.Length);
			var (sizeX, sizeY) = size;
			var map = new char[sizeX,sizeY];
			for (var y = 0; y < sizeY; y++)
			{
				var line = lines[y];
				for (var x = 0; x < sizeX; x++)
				{
					map[x, y] = line[x];
				}
			}

            // var mapToMap = new Dictionary<int, char[,]>();
            var mapToT = new Dictionary<string, long>();
            long? loopSize = null;
			for ( long t = 0; t < time; t++ )
			{
                if (!loopSize.HasValue)
                {
                    // state saving
                    var hash = ToHash(map, size);
                    if (mapToT.ContainsKey(hash))
                    {
                        // found the repeation
                        var tPrev = mapToT[hash];
                        // the full loop is tPrev == t (ie 2 and 4) means only two states: 2, 3
                        loopSize = t - tPrev;
                        Console.WriteLine($"Identified loop size: {loopSize} on time: {t}");
                        // Console.WriteLine(hash);
                        while (t < (time - loopSize))
                        {
                            t += loopSize.Value; // rewind forward
                        }
                    }
                    else
                    {
                        mapToT.Add(hash, t);
                        // mapToMap.Add(hash, map);
                    }
                }

                var map1 = (char[,])map.Clone();
				for (var y = 0; y < sizeY; y++)
				{
					for (var x = 0; x < sizeX; x++)
					{
						/*
						 * An open acre will become filled with trees if three or more adjacent acres contained trees. 
						 * Otherwise, nothing happens.
						 * 
						 * An acre filled with trees will become a lumberyard if three 
						 * or more adjacent acres were lumberyards. Otherwise, nothing happens.
						 * 
						 * An acre containing a lumberyard will remain a lumberyard 
						 * if it was adjacent to at least one other lumberyard and at least one acre containing trees. 
						 * Otherwise, it becomes open.
						 */
						var around = Around(map, size, (x, y)).ToArray();
						var grouped = around.GroupBy(s => s).ToArray();
						char acre = map[x, y];
						switch ( acre )
						{
							case Ground:
								if ( grouped.Any(g=>g.Key==Tree && g.Count() >= 3) )
								{
									acre = Tree;
								}
								break;
							case Tree:
								if (grouped.Any(g => g.Key == Lumberyard && g.Count() >= 3))
								{
									acre = Lumberyard;
								}
								break;
							case Lumberyard:
								if (   grouped.Any(g => g.Key == Lumberyard && g.Count() >= 1)
									&& grouped.Any(g => g.Key == Tree && g.Count() >= 1) )
								{
									acre = Lumberyard;
								}
								else
								{
									acre = Ground;
								}
								break;
							default: Assert.Fail("unexpected thing on map"); break;
						}
						map1[x, y] = acre;
					}
				}			   
				map = map1; // full substition
			}

			int lumberCount = 0;
			int treeCount = 0;
			// now counts
			for (var y = 0; y < sizeY; y++)
			{
				for (var x = 0; x < sizeX; x++)
				{
					switch(map[x,y])
					{
						case Lumberyard: lumberCount++; break;
						case Tree: treeCount++; break;
					}
				}
			}
			ClassicAssert.AreEqual( expectedAnswer , lumberCount * treeCount, "tree count");
		}

		public IEnumerable<char> Around(char[,] map, (int X, int Y) size, (int X, int Y) at )
		{
			for ( int x = Math.Max(at.X - 1, 0); x <= Math.Min(size.X - 1, at.X + 1); x++ )
			{
				for (int y = Math.Max(at.Y - 1, 0); y <= Math.Min(size.Y - 1, at.Y + 1); y++)
				{
					if ( (x,y) != at )
					{
						yield return map[x, y];
					}
				}
			}
		}
	}
}
