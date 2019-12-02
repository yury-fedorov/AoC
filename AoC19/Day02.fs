module Day02

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

[<Theory>]
[<InlineData(0,70,3,3)>]
let SampleTest pos0 
