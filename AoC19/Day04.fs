module Day04

open Xunit

let IsValid ( pwd : int ) = 
    let c = pwd.ToString().ToCharArray()
    if (Array.length c <> 6 ) then false
    else
        let shifted = Seq.tail c
        // sure tail is necessary? first vs last not to compare?
        let z = Seq.take 5 (Seq.zip c shifted |> Seq.toArray ) |> Seq.toList 
        let isNotDecreasing = z |> Seq.map (fun (a,b) -> a <= b ) |> (Seq.contains false) <> true
        let isWithPair = z |> Seq.map (fun (a,b) -> a = b ) |> Seq.contains true
        isNotDecreasing && isWithPair

// let Normalize ( pwd : int ) = pwd.ToString().ToCharArray() |>

[<Theory>]
[<InlineData(122345, true)>]
[<InlineData(111123, true)>]
[<InlineData(135679, false)>]
[<InlineData(111111, true)>]
[<InlineData(223450, false)>]
[<InlineData(123789, false)>] // does not meet these criteria (no double)
[<InlineData(136760, false)>] // from
[<InlineData(595730, false)>] // till
let IsValidTest pwd exp = Assert.Equal( ( IsValid pwd ), exp )

let Question1 from till = seq { from .. till } |> Seq.length

[<Theory>]
[<InlineData(111111,111113, 3)>]
[<InlineData(136760,595730, -1)>] // 458971 - too high
let Question1Test from till exp =
    Assert.Equal( (Question1 from till), exp )


