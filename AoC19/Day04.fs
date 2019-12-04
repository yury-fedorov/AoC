module Day04

open Xunit

let IsValid ( pwd : int ) = 
    let c = pwd.ToString().ToCharArray()
    let z = Seq.take 5 (Seq.zip c (Seq.tail c)) |> Seq.toList 
    let isNotDecreasing = not ( z |> Seq.map (fun (a,b) -> a <= b ) |> Seq.contains false )
    let isWithPair = z |> Seq.map (fun (a,b) -> a = b ) |> Seq.contains true
    isNotDecreasing && isWithPair

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

let CountPredicate from till pred = seq { from .. till } |> Seq.filter (fun n -> pred n) |> Seq.length

let Question1 from till = CountPredicate from till IsValid

[<Theory>]
[<InlineData(111111,111113, 3)>]
[<InlineData(136760,595730, 1873)>] // 458971 - too high
let Question1Test from till exp = Assert.Equal( (Question1 from till), exp )

let CountSeq ( s : array<char> ) = 
    let mutable cur = '?'
    let mutable count = 0
    seq { 
        for c in s do
            if ( cur = c ) then count <- count + 1
            else
                if ( count > 0 ) then yield (cur,count)
                count <- 1
                cur <- c

        yield (cur,count)
    }

[<Theory>]
[<InlineData(112233, 3)>]
[<InlineData(123444, 4)>]
[<InlineData(123333, 3)>]
[<InlineData(111111, 1)>]
[<InlineData(110011, 3)>]
let TestGroupCount pwd eg =
    let p = pwd.ToString().ToCharArray()
    Assert.Equal( ( CountSeq p ) |> Seq.length, eg )

let IsValid2 ( pwd : int ) = 
    let c = pwd.ToString().ToCharArray()
    let cc = CountSeq c |> Seq.toList
    let isGood = cc |> Seq.forall ( fun (_,n) -> (n <> 3 && n <> 5) )
    isGood && ( IsValid pwd )

[<Theory>]
[<InlineData(112233, true)>]
[<InlineData(123444, false)>]
[<InlineData(111122, true)>]
[<InlineData(123333, true)>]
[<InlineData(111111, true)>]
[<InlineData(110011, false)>]
let IsValidTest2 pwd exp = Assert.Equal( exp, ( IsValid2 pwd ) )

let Question2 from till = CountPredicate from till IsValid2

[<Theory>]
[<InlineData(136760,595730, -1)>] // 1115 - too low
let Question2Test from till exp = Assert.Equal( exp, (Question2 from till) )

