module Day05

open System
open System.IO
open Xunit
open Day02 // initial impletementation

let CodeIn = 3
let CodeOut = 4

let In : int = 
    printf "Input:"
    Console.ReadLine() |> int

let Out arg = printf "%d" arg 

let GetOperation (instruction:int) : int = instruction % 100 // last 2 digits

[<Theory>]
[<InlineData(1002, 2)>]
let TestGetOperation instruction expectedOperation = 
    Assert.Equal( expectedOperation, GetOperation instruction )