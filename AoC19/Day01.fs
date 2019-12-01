module Day01

open System.IO
open Xunit


let GetFuel(mass: int): int = (mass / 3) - 2

[<Theory>]
[<InlineData(12,2)>]
[<InlineData(14, 2)>]
[<InlineData(1969, 654)>]
[<InlineData(100756, 33583)>]
let ``My test 1`` mass expected = Assert.Equal(GetFuel(mass), expected)

let rec GetFuel2 (currentMass : int) (currentFuel : int) =
    let deltaFuel = GetFuel currentMass
    if (deltaFuel > 0) 
    then GetFuel2 deltaFuel (currentFuel + deltaFuel) 
    else currentFuel

[<Theory>]
[<InlineData(14,2)>]
[<InlineData(1969, 966)>]
[<InlineData(100756,50346)>]
let Test2 mass expected = Assert.Equal(GetFuel2 mass 0, expected)

let fileName = "../../../Day01.txt"

let GetData = File.ReadAllLines(fileName) |> Array.map int

[<Theory>]
[<InlineData(3368364)>]
let Answer1 expected =
    let sum = GetData |> Array.map GetFuel |> Array.sum
    Assert.Equal( expected, sum )

[<Theory>]
[<InlineData(5049684)>]
let Answer2 expected =
    let sum = GetData |> Array.map( fun mass -> GetFuel2 mass 0 ) |> Array.sum
    Assert.Equal( expected, sum )