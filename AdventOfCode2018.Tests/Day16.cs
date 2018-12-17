using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace AdventOfCode2018.Day16
{
	public abstract class Instruction
	{
		public int[] Registers;
		public const int A = 1;
		public const int B = 2;
		public const int C = 3;
		public abstract void Execute( int [] instruction );
		public readonly string Name;
		protected Instruction(string name) { Name = name; }
	}

	// (add register) stores into register C the result of adding register A and register B.
	public class OpRegister : Instruction
	{
		public readonly Func<int, int, int> _method;
		public OpRegister(string name, Func<int,int,int> method ) : base(name) {
			_method = method;
		}

		public override void Execute(int[] instruction)
		{
			Registers[instruction[C]] = _method( Registers[instruction[A]], Registers[instruction[B]] );
		}
	}

	public class OpImmediate1 : Instruction
	{
		public readonly Func<int, int, int> _method;
		public OpImmediate1(string name, Func<int, int, int> method) : base(name)
		{
			_method = method;
		}

		public override void Execute(int[] instruction)
		{
			Registers[instruction[C]] = _method(instruction[A], Registers[instruction[B]] );
		}
	}

	// addi (add immediate) stores into register C the result of adding register A and value B.
	public class OpImmediate2 : Instruction
	{
		public readonly Func<int, int, int> _method;
		public OpImmediate2(string name, Func<int, int, int> method) : base(name)
		{
			_method = method;
		}

		public override void Execute(int[] instruction)
		{
			Registers[instruction[C]] = _method(Registers[instruction[A]], instruction[B]);
		}
	}

	public class Sample
	{
		public int[] Before;
		public int[] Operation;
		public int[] After;
	}

	public class Day16
	{
		public readonly Instruction [] Instructions = {
			// Addition
			new OpRegister ("addr", (a,b)=>a+b ),
			new OpImmediate2("addi", (a,b)=>a+b ),
			// Multiplication
			new OpRegister ("mulr", (a,b)=>a*b ),
			new OpImmediate2("muli", (a,b)=>a*b ),
			// Bitwise AND
			new OpRegister ("banr", (a,b)=>a&b ),
			new OpImmediate2("bani", (a,b)=>a&b ),
			// Bitwise OR
			new OpRegister ("borr", (a,b)=>a|b ),
			new OpImmediate2("bori", (a,b)=>a|b ),
			// Assignment: (set register) copies the contents of register A into register C. (Input B is ignored.)
			new OpRegister  ("setr", (a,b)=>a ),
			new OpImmediate2("seti", (a,b)=>a ),
			// Greater-than testing
			new OpRegister   ("gtrr", (a,b)=>a>b?1:0 ),
			new OpImmediate1 ("gtir", (a,b)=>a>b?1:0 ),
			new OpImmediate2 ("gtri", (a,b)=>a>b?1:0 ),
			// Equality testing
			new OpRegister   ("eqrr", (a,b)=>a==b?1:0 ),
			new OpImmediate1 ("eqir", (a,b)=>a==b?1:0 ),
			new OpImmediate2 ("eqri", (a,b)=>a==b?1:0 ),
		};

		public int [] ToArray( string line, string pattern )
		{
			var matches = Regex.Matches(line, pattern);
			foreach (Match match in matches)
			{
				return new[] {
					Convert.ToInt32(match.Groups[1].Value),
					Convert.ToInt32(match.Groups[2].Value),
					Convert.ToInt32(match.Groups[3].Value),
					Convert.ToInt32(match.Groups[4].Value) };
			}
			throw new Exception("unexpected flow");
		}

		[TestCase("Day16.txt")]
		public void TestSample3(string file)
		{
			var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file));

			var patternBefore = @"Before: .([0-9]+), ([0-9]+), ([0-9]+), ([0-9]+).";
			var patternOperation = @"([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+)";
			var patternAfter  = @"After:  .([0-9]+), ([0-9]+), ([0-9]+), ([0-9]+).";

			var samples = new List<Sample>();
			var latestSampleStart = -1;
			for ( int i = 0; i < lines.Length; i++)
			{
				var line = lines[i];
				if ( line.StartsWith( "Before" ) )
				{
					// found the new start of sample
					/*
					 * Before: [2, 3, 2, 2]
					 * 15 3 2 2
					 * After:  [2, 3, 4, 2]
					 */
					var sample = new Sample();
					sample.Before = ToArray(line, patternBefore);
					sample.Operation = ToArray(lines[i + 1], patternOperation);
					sample.After = ToArray(lines[i + 2], patternAfter);
					samples.Add(sample);
					latestSampleStart = i;
				}
			}

			Assert.True( samples.Count > 10 );

			Assert.AreEqual(Instructions.Length, samples.Select(s=>s.Operation.First()).Distinct().Count(), 
				"we expect that 1 code for one instruction (1 to 1 mapping)" );

			// now checking
			var mapCodeOperation = new Dictionary<int, Instruction>(); // defined
			var mapCodeOptions = new Dictionary<int, HashSet<string>>();
			int count = 0;
			foreach ( var sample in samples )
			{
				var set = new HashSet<string>();
				foreach ( var op in Instructions )
				{
					op.Registers = (int [])sample.Before.Clone();
					op.Execute(sample.Operation);
					var matching = op.Registers.SequenceEqual(sample.After);
					if (matching) set.Add(op.Name);
				}
				count += set.Count() >= 3 ? 1 : 0;
				var opcode = sample.Operation.First();
				if (mapCodeOperation.ContainsKey(opcode))
				{
					var op = mapCodeOperation[opcode];
					op.Registers = (int[])sample.Before.Clone();
					op.Execute(sample.Operation);
					var matching = op.Registers.SequenceEqual(sample.After);
					Assert.True(matching, "all samples must match detected operation");
				}
				else
				{ 
					if (set.Count() == 1)
					{
						mapCodeOperation.Add(opcode, Instructions.Single(i => i.Name == set.Single()));
					}
					else if ( mapCodeOptions.ContainsKey(opcode) )
					{
						var existingSet = mapCodeOptions[opcode];
						var minusDefined = existingSet.Except(mapCodeOperation.Values.Select(i => i.Name)).ToHashSet();
						var forIntersection = minusDefined.Any() ? minusDefined : existingSet;
						var newScope = forIntersection.Intersect(set).ToHashSet();
						// var newScope = existingSet.Intersect(set).ToHashSet();
						if ( !newScope.Any())
						{
							Assert.True(newScope.Count() > 0, "scope can not be reduced to zero");
							// mapCodeOperation[opcode] = null;
							// Console.Error.WriteLine( $"{opcode} is unknown" );
						}
							
						if ( newScope.Count() == 1 )
						{
							// we identified the operation!
							mapCodeOperation.Add(opcode, Instructions.Single(i => i.Name == newScope.Single()));
						}
						else
						{
							mapCodeOptions[opcode] = newScope.ToHashSet();
						}
					}
					else
					{
						// var existingSet = set;
						var intersect = set.Except(mapCodeOperation.Values.Select(i => i.Name)).ToHashSet();
						var newScope = intersect.Any() ? intersect : set;
						if (!newScope.Any())
						{
							Assert.True(newScope.Count() > 0, "scope can not be reduced to zero");
							// mapCodeOperation[opcode] = null;
							// Console.Error.WriteLine($"{opcode} is unknown");
						}
						/*
						*/
						if (newScope.Count() == 1)
						{
							// we identified the operation!
							mapCodeOperation.Add(opcode, Instructions.Single(i => i.Name == newScope.Single()));
						}
						else
						{
							mapCodeOptions.Add( opcode, newScope);
						}
					}
				}				
			}

			Assert.AreEqual(544, count, "answer1");

			// now detect code to operation

			// Assert.AreEqual(Instructions.Length, mapCodeOperation.Count, "All has to be mapped");

			int[] registers = { 0, 0, 0, 0 };
			for (int i = latestSampleStart + 3; i < lines.Length; i++ )
			{
				var line = lines[i];
				if (string.IsNullOrEmpty(line.Trim())) continue;
				var operation = ToArray(line, patternOperation);
				var opcode = operation.First();
				if ( mapCodeOperation.ContainsKey(opcode) )
				{
					var op = mapCodeOperation[opcode];
					op.Registers = registers;
					op.Execute(operation);
					registers = op.Registers;
				}
				else
				{
					// we have options
					// we try to execute the first
					var before = (int[])registers.Clone();
					int [] after = null;
					foreach( var opname in mapCodeOptions[opcode] )
					{
						var op = Instructions.Single(x => x.Name == mapCodeOptions[opcode].First());
						op.Registers = registers;
						op.Execute(operation);
						if (after == null) after = (int[])op.Registers.Clone();
						else
						{
							var matching = op.Registers.SequenceEqual(after);
							Assert.True(matching, "ambiguity in data processing");
						}
					}
					registers = after;
				}
			}
			Assert.AreEqual(0, registers.First(), "answer 2");
		}
	}
}
