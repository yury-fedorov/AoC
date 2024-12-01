using NUnit.Framework;
using NUnit.Framework.Legacy;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

namespace AdventOfCode2018.Day16
{
    public enum Code { addr, addi, mulr, muli, banr, bani, borr, bori, setr, seti, gtrr, gtir, gtri, eqrr, eqir, eqri }

    public abstract class Instruction
	{
		public const int A = 1;
		public const int B = 2;
		public const int C = 3;

	    public readonly Code Name;
	    public readonly Func<long, long, long> Method;
	    public readonly Func<int[], long[], int, long> ArgA;
	    public readonly Func<int[], long[], int, long> ArgB;

	    public static readonly Func<int[], long[], int, long> Value = (i, r, a) => i[a];
	    public static readonly Func<int[], long[], int, long> Register = (i, r, a) => r[i[a]];

        protected Instruction(Code name, Func<long, long, long> method, Func<int[], long[], int, long> a, Func<int[], long[], int, long> b)
	    {
	        Name = name;
	        Method = method;
	        ArgA = a;
	        ArgB = b;
	    }

        public virtual long Execute(int[] instruction, long[] registers)
			=> registers[instruction[C]] = Method(ArgA(instruction, registers, A), ArgB(instruction, registers, B));
    }

	// (add register) stores into register C the result of adding register A and register B.
	public class OpRegister : Instruction
	{
		public OpRegister(Code name, Func<long,long,long> method ) : base(name, method, Register, Register ) {}
	}

	public class OpImmediate1 : Instruction
	{
		public OpImmediate1(Code name, Func<long, long, long> method) : base(name, method, Value, Register) {}
	}

	// addi (add immediate) stores into register C the result of adding register A and value B.
	public class OpImmediate2 : Instruction
	{
		public OpImmediate2(Code name, Func<long, long, long> method) : base(name, method, Register, Value) {}
	}

    public class OpImmediate12 : Instruction
    {
        public OpImmediate12(Code name, Func<long, long, long> method) : base(name, method, Value, Value) {}
    }


    public class Sample
	{
		public long[] Before;
		public int[] Operation;
		public long[] After;
	}

	public class Day16
	{
	    public static readonly Func<long, long, long> add = (a, b) => a + b;
	    public static readonly Func<long, long, long> mul = (a, b) => a * b;
	    public static readonly Func<long, long, long> ban = (a, b) => a & b;
	    public static readonly Func<long, long, long> bor = (a, b) => a | b;
	    public static readonly Func<long, long, long> set = (a, b) => a ;
	    public static readonly Func<long, long, long> gt  = (a, b) => a > b ? 1 : 0;
	    public static readonly Func<long, long, long> eq  = (a, b) => a == b ? 1 : 0;

        public static readonly Instruction [] Instructions = {
			// Addition
			new OpRegister (Code.addr, add ),
			new OpImmediate2(Code.addi, add ),
			// Multiplication
			new OpRegister (Code.mulr, mul ),
			new OpImmediate2(Code.muli, mul ),
			// Bitwise AND
			new OpRegister (Code.banr, ban ),
			new OpImmediate2(Code.bani, ban ),
			// Bitwise OR
			new OpRegister (Code.borr, bor ),
			new OpImmediate2(Code.bori, bor ),
			// Assignment: (set register) copies the contents of register A into register C. (Input B is ignored.)
			new OpImmediate2 (Code.setr, set ), // setr (set register) copies the contents of register A into register C. (Input B is ignored.)
			new OpImmediate12(Code.seti, set ), // seti (set immediate) stores value A into register C. (Input B is ignored.)
			// Greater-than testing
			new OpRegister   (Code.gtrr, gt ),
			new OpImmediate1 (Code.gtir, gt ),
			new OpImmediate2 (Code.gtri, gt ),
			// Equality testing
			new OpRegister   (Code.eqrr, eq ),
			new OpImmediate1 (Code.eqir, eq ),
			new OpImmediate2 (Code.eqri, eq )
		};

	    public static readonly Dictionary<Code, Instruction> MapCodeInstruction = Instructions.ToDictionary(i => i.Name, i => i);

	    public static readonly HashSet<Code> FullSet = MapCodeInstruction.Keys.ToHashSet();


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

	    public static string ToText<T>(IEnumerable<T> a) => string.Join(',', a.Select(e => Convert.ToString(e)).ToArray());

	    public static HashSet<Code> GoodCodes(Sample sample)
	    {
	        var set = new HashSet<Code>();
	        foreach (var op in Instructions)
	        {
	            var registers = (long[])sample.Before.Clone();
	            op.Execute(sample.Operation, registers);
	            var matching = registers.SequenceEqual(sample.After);
	            if (matching) set.Add(op.Name);
	        }
            ClassicAssert.IsNotEmpty(set,"expected at least one alternative for each sample");
	        return set;
	    }

        [TestCase("Day16/input.txt")]
		public void Solution(string file)
		{
			var lines = File.ReadAllLines(Path.Combine(App.Directory, file));

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
					sample.Before = ToArray(line, patternBefore).Select( Convert.ToInt64 ).ToArray();
					sample.Operation = ToArray(lines[i + 1], patternOperation);
					sample.After = ToArray(lines[i + 2], patternAfter).Select(Convert.ToInt64).ToArray();
                    samples.Add(sample);
					latestSampleStart = i;
				}
			}

			ClassicAssert.True( samples.Count > 10 );

			ClassicAssert.AreEqual(Instructions.Length, samples.Select(s=>s.Operation.First()).Distinct().Count(), 
				"we expect that 1 code for one instruction (1 to 1 mapping)" );

			// now checking
			// var mapCodeOperation = new Dictionary<int, Instruction>(); // defined
			var mapCodeOptions = new Dictionary<int, HashSet<Code>>();

            ClassicAssert.AreEqual(16, FullSet.Count(), "number of known operations");
		    foreach (var code in samples.Select(s => s.Operation.First()).Distinct())
		    {
		        mapCodeOptions.Add(code,FullSet);
		    }            

			int count = 0;
			foreach ( var sample in samples )
			{
			    var set = GoodCodes(sample);
				count += set.Count() >= 3 ? 1 : 0;
				var opcode = sample.Operation.First();
			    var intersection = mapCodeOptions[opcode].Intersect(set).ToHashSet();
                mapCodeOptions[opcode] = intersection;
                ClassicAssert.IsNotEmpty(intersection);
			}

			ClassicAssert.AreEqual(544, count, "answer1");

            // now detect code to operation

            // Assert.AreEqual(Instructions.Length, mapCodeOperation.Count, "All has to be mapped");

			/*
		    foreach (var co in mapCodeOptions.OrderBy(a=>a.Value.Count))
		    {
		        Console.WriteLine($"{co.Key} - [{ToText(co.Value.OrderBy(x => x))}]");
		    }
			*/

            var mapCodeOperation = new Dictionary<int, Code>();
            
            while ( true )
            {
                var newOneOptionCases = mapCodeOptions.Where(a => a.Value.Count() == 1)
                    .ToDictionary(a => a.Key, a => a.Value.Single()).ToArray();

                if (newOneOptionCases.Length == 0) break;

                // new mappging
                // and remove all 1 element maps
                foreach (var detected in newOneOptionCases)
                {
                    mapCodeOperation.Add(detected.Key, detected.Value);
                    mapCodeOptions.Remove(detected.Key);
                    // remove all known from alternatives
                    foreach (var a in mapCodeOptions)
                    {
                        a.Value.RemoveWhere(s => s == detected.Value);
                        if (!a.Value.Any())
                        {                            
                            var set = FullSet;
                            foreach (var s in samples.Where(s => s.Operation.First() == a.Key))
                            {
                                var variants = GoodCodes(s);
                                // Console.WriteLine( $"[{ToText(s.Before)}] ({ToText(s.Operation)}) [{ToText(s.After)}] ({ToText(variants.OrderBy(x=>x))})" );
                                set = set.Intersect(variants).ToHashSet();
                            }
                            // Console.WriteLine($"({ToText(set.OrderBy(x => x))})");
                            // Assert.IsNotEmpty(a.Value, "option can not be empty");
                        }
                    }
                }
            }
			/*
		    foreach (var co in mapCodeOperation)
		    {
		        Console.WriteLine($"{co.Key} - [{co.Value}]");
		    }
			foreach (var co in mapCodeOptions.OrderBy(a => a.Value.Count))
		    {
		        Console.WriteLine($"{co.Key} - [{ToText(co.Value.OrderBy(x => x))}]");
            }
			*/

			ClassicAssert.IsEmpty(mapCodeOptions, "expected full decoding");

            long[] registers = { 0, 0, 0, 0 };
			for (int i = latestSampleStart + 3; i < lines.Length; i++ )
			{
				var line = lines[i];
				if (string.IsNullOrEmpty(line.Trim())) continue;
				var operation = ToArray(line, patternOperation);
				var opcode = operation.First();
				var code = mapCodeOperation[opcode];
			    var op = MapCodeInstruction[code];
				op.Execute(operation, registers);
			}
			ClassicAssert.AreEqual(600, registers.First(), "answer 2");
		}
	}
}
