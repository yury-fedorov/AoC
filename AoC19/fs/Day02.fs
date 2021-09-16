module Day02

open System.IO
open Xunit

let Sample = "1,9,10,3,2,3,11,0,99,30,40,50"

let IntArr (s : string) = s.Split ',' |> Array.map int

let CodeAdd = 1
let CodeMul = 2
let CodeEnd = 99

let Add a b = a + b
let Mul a b = a * b

type MathOperation =
    { Result        : int
      ResultIndex   : int
      Position      : int }

type Operation =
    | End 
    | Math of MathOperation

let ExecOp pos (code: array<int>) : Operation = 
    let codeOp = code.[(pos)]
    if codeOp = CodeEnd then
        End
    else
        // 1 and 2
        let f = if (codeOp = CodeAdd) then Add else Mul
        let arg1Index = code.[(pos + 1)] 
        let arg2Index = code.[(pos + 2)]
        let resIndex  = code.[(pos + 3)]
        let arg1 = code.[arg1Index]
        let arg2 = code.[arg2Index]
        let res = (f arg1 arg2)
        Math { Result = res; ResultIndex = resIndex; Position = (pos + 4); }

let rec Exec pos (code: array<int>) =
    let op = ExecOp pos code
    match op with
    | End -> code
    | Math m ->
        code.[(m.ResultIndex)] <- m.Result
        Exec m.Position code

[<Theory>]
[<InlineData(0,70,3,4)>]
let SampleTest pos0 expR expRI expPos1 = 
    let op = ExecOp pos0 (IntArr Sample)
    match op with 
    | End -> Assert.True(false)
    | Math m -> 
        Assert.Equal(expR, m.Result)
        Assert.Equal(expRI, m.ResultIndex)
        Assert.Equal(expPos1, m.Position)

[<Theory>]
[<InlineData("1,0,0,0,99","2,0,0,0,99")>]
[<InlineData("2,3,0,3,99","2,3,0,6,99")>]
[<InlineData("2,4,4,5,99,0","2,4,4,5,99,9801")>]
[<InlineData("1,1,1,4,99,5,6,0,99","30,1,1,4,2,5,6,0,99")>]
let SampleRun code0 code1 =
    let c0 = IntArr code0
    let c1 : array<int> = Exec 0 c0
    let expC1 : array<int> = IntArr code1
    let i = [| 0 .. (Array.length c1) - 1 |]
    i |> Array.map ( fun i -> Assert.Equal(c1.[i],expC1.[i]) )


let fileName = "../../../../data/Day02.txt"

[<Theory>]
[<InlineData(3716250)>]
let Answer1 exp =
    let cs = File.ReadAllText(fileName)
    let code = IntArr cs
    code.[1] <- 12
    code.[2] <- 2
    let r = Exec 0 code
    Assert.Equal(r.[0],exp)


let Result2 = 19690720

let Calc (code: array<int>) noun verb =
    code.[1] <- noun
    code.[2] <- verb
    let r = Exec 0 code
    r.[0]

let Search (code: array<int>) = 
    let l = (Array.length code) - 1
    seq { 
        for n in 0..l do
            for v in 0..l do
                let r = Calc (Array.copy code) n v
                yield (n,v,r) 
    }

let Find (code: array<int>) =
    Seq.find (fun (n,v,r) -> r = Result2) (Search code)

let A2 n v = (n * 100) + v
    
[<Theory>]
[<InlineData(6472)>] // 6471 - no
let Answer2 er =
    let cs = File.ReadAllText(fileName)
    let code = IntArr cs
    let (n,v,r) = Find code
    Assert.Equal(er, (A2 n v))


[<Theory>]
[<InlineData(64,72)>] // 6471 - no
let Test2 n v =
    let cs = File.ReadAllText(fileName)
    let code = IntArr cs
    let r = Calc code n v
    Assert.Equal(Result2, r) 
