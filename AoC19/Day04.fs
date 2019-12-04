module Day04

open System.IO
open Xunit

let IsValid ( pwd : int ) = 
    let c = pwd.ToString().ToCharArray()
    if (Array.length c <> 6 ) then false
    else
        let shifted = Array.tail c 
        true

[<Theory>]
[<InlineData(111111, true)>]
[<InlineData(223450, false)>]
[<InlineData(123789, false)>] // does not meet these criteria (no double)
let IsValidTest pwd exp = Assert.Equal( ( IsValid pwd ), exp )

let Question1 from till = 
    seq { from .. till } |> Seq.length

[<Theory>]
[<InlineData(111111,111113, 3)>]
[<InlineData(136760,595730, -1)>]
let Question1Test from till exp =
    Assert.Equal( (Question1 from till), exp )


