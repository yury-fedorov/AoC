module Day07

open System.IO
open Xunit

let fileName = "../../../Day07.txt"

let GetData = File.ReadAllLines(fileName)




(*
    using System.Text.RegularExpressions;

    var patternOperation = @"([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+)";

    var matches = Regex.Matches(line, pattern);
    foreach (Match match in matches)
    {
        return new[] {
            Convert.ToInt32(match.Groups[1].Value),
            Convert.ToInt32(match.Groups[2].Value),
            Convert.ToInt32(match.Groups[3].Value),
            Convert.ToInt32(match.Groups[4].Value) };
    }
*)

