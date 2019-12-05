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

let GetOpCode (instruction:int) : int = instruction % 100 // last 2 digits

[<Theory>]
[<InlineData(1002, 2)>]
let TestGetOperation instruction expectedOperation = 
    Assert.Equal( expectedOperation, GetOpCode instruction )

type Operation = 
    | End
    | Add of a: int * b: int * r: int
    | Mul of a: int * b: int * r: int
    | In of a: int
    | Out of a: int

let OperationLength (o:Operation) =
    match o with 
    | End -> 1
    | Add (_,_,_) -> 4
    | Mul (_,_,_) -> 4
    | In _ -> 2
    | Out _-> 2

let GetArgsCount (o:Operation) =
    let rawCount = ( OperationLength o ) - 2 // codop and result
    max 0 rawCount // for End rawCount is negative

let ReadOp pos (code: array<int>) : Operation = 
    let instruction = code.[pos]
    let opcode = GetOpCode instruction
    match opcode with
    | CodeEnd -> End
    | CodeAdd -> Add( code.[pos+1], code.[pos+2], code.[pos+3] )