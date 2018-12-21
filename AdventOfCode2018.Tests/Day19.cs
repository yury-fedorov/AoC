using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using AdventOfCode2018.Day16;
using NUnit.Framework;

namespace AdventOfCode2018.Day19
{
	/*
	 The instruction pointer contains 0, and so the first instruction is executed (seti 5 0 1). 
	 It updates register 0 to the current instruction pointer value (0), sets register 1 to 5, 
	 sets the instruction pointer to the value of register 0 (which has no effect, 
	 as the instruction did not modify register 0), and then adds one to the instruction pointer.
	public class Seti : Instruction
	{
		public readonly int _ip;
		public Seti( int ip ) : base(Code.seti,set, )
		{
			_ip = ip;
		}

		public override int Execute(int[] instruction, int[] registers)
		{
			// It updates register 0 to the current instruction pointer value (0),

			return registers[instruction[C]] = Method(ArgA(instruction, registers, A), ArgB(instruction, registers, B));
		}
	}
	*/

	public class Day19
	{
		public int GetIP(string line)
		{
			const string patternIp = @"#ip ([0-5])";
			var matches = Regex.Matches(line, patternIp);
			foreach (Match match in matches)
			{
				
				return Convert.ToInt32(match.Groups[1].Value);
			}
			throw new Exception("unexpected flow");
		}

		public int[] ToArray(string line)
		{
			const string pattern = @"([a-z]+) ([0-9]+) ([0-9]+) ([0-9]+)";
			var matches = Regex.Matches(line, pattern);
			foreach (Match match in matches)
			{
				Code code;
				var success = Enum.TryParse<Code>(match.Groups[1].Value, out code);
				Assert.True(success, "expected to parse successfully");

				return new[] {
					Convert.ToInt32(code),
					Convert.ToInt32(match.Groups[2].Value),
					Convert.ToInt32(match.Groups[3].Value),
					Convert.ToInt32(match.Groups[4].Value) };
			}
			throw new Exception("unexpected flow");
		}
		
		public string ToString<T>( T[] a ) 
			=> string.Join(',', a.Select( o => o.ToString() ).ToArray());

		[TestCase("Day19Sample.txt", 6)]
		[TestCase("Day19.txt", -1)]
		public void Test1(string file, int reg0halt)
		{
			var lines = File.ReadAllLines(Path.Combine(Day1Test.Directory, file));
			int ipBinding = GetIP(lines.First());
			int ip=0;
			var code = new List<int[]>();
			for( int i = 1; i < lines.Length; i++)
			{
				code.Add( ToArray(lines[i]) );
			}
			var registers = new int[6] { 0, 0, 0, 0, 0, 0 };
			while ( ip < code.Count() )
			{
				var curInstruction = code[ip];
				var opcode = (Code)curInstruction[0];
				var instruction = Day16.Day16.MapCodeInstruction[opcode];
				Console.WriteLine($"[{ToString(registers)}] {ip} {opcode} {ToString(curInstruction)}");
				try
				{
					instruction.Execute(curInstruction, registers);
				}
				catch ( Exception e)
				{
					Assert.Fail(e.Message);
				}
				Console.WriteLine($"{ToString(registers)}");
				ip++;
			}
			// correction of increment done out of scope
			ip--;
			
			Assert.AreEqual(reg0halt, registers[0]);
		}
	}
}
