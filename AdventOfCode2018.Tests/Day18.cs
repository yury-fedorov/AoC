using NUnit.Framework;
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

		// [TestCase("Day18.txt",10,894,576)]
		[TestCase("Day18Sample.txt",10,37,31)]
		[TestCase("Day18Sample.txt", 1000000000, 0, 0)] // task 2
		public void Test1(string file, int time, int wood, int lumber)
		{
			var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file));
			var size = (lines[0].Length, lines.Length);
			var map = new char[size.Item1,size.Item2];
			for (var y = 0; y < size.Item2; y++)
			{
				var line = lines[y];
				for (var x = 0; x < size.Item1; x++)
				{
					map[x, y] = line[x];
				}
			}

			for ( int t = 0; t < time; t++ )
			{
				var map1 = (char[,])map.Clone();
				for (var y = 0; y < size.Item2; y++)
				{
					for (var x = 0; x < size.Item1; x++)
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

				if ( t == -1 )
				{
					for ( int y = 0; y < size.Item2; y++ )
					{
						var s = new StringBuilder();
						for (int x = 0; x < size.Item1; x++)
							s.Append(map1[x, y]);
						Console.WriteLine(s);
					}
				}
							   
				map = map1; // full substition
			}

			int lumberCount = 0;
			int treeCount = 0;
			// now counts
			for (var y = 0; y < size.Item2; y++)
			{
				for (var x = 0; x < size.Item1; x++)
				{
					switch(map[x,y])
					{
						case Lumberyard: lumberCount++; break;
						case Tree: treeCount++; break;
					}
				}
			}
			// Assert.AreEqual(lumber, lumberCount, "lumber count");
			// Assert.AreEqual(wood, treeCount, "wood count");
			Assert.AreEqual(lumber*wood, lumberCount * treeCount); // wrong 656366
		}

		public IEnumerable<char> Around(char[,] map, (int,int) size, (int,int) at )
		{
			for ( int x = Math.Max(at.Item1 - 1, 0); x <= Math.Min(size.Item1 - 1, at.Item1 + 1); x++ )
			{
				for (int y = Math.Max(at.Item2 - 1, 0); y <= Math.Min(size.Item2 - 1, at.Item2 + 1); y++)
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
