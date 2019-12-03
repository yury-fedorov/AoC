module Day03

open System.IO
open Xunit

type Direction =
    | Left of int
    | Right of int
    | Up of int
    | Down of int
     
type Point = { X : int; Y : int } // tuple?

let Trace (p0:Point) (d:Direction) : seq<Point> =
    match d with
    | Left  dx -> [ 1 .. dx ] |> Seq.map ( fun x -> { X=p0.X-x; Y=p0.Y} )
    | Right dx -> [ 1 .. dx ] |> Seq.map ( fun x -> { X=p0.X+x; Y=p0.Y} )
    | Up    dy -> [ 1 .. dy ] |> Seq.map ( fun y -> { X=p0.X; Y=p0.Y+y} )
    | Down  dy -> [ 1 .. dy ] |> Seq.map ( fun y -> { X=p0.X; Y=p0.Y-y} )

let Point0 : Point = {X=0; Y=0}

let ToDirection (s:string) : Direction = 
    let a = s.ToCharArray()
    let d = Array.head a
    let n = Array.tail a |> System.String |> int
    match d with
    | 'L' -> Left n
    | 'R' -> Right n
    | 'U' -> Up n
    | 'D' -> Down n

[<Theory>]
[<InlineData("R12", true)>]
[<InlineData("R4", false)>]
[<InlineData("U3", false)>]
let TestR12 s exp = Assert.Equal( (Right 12) = (ToDirection s), exp )

let ToDirectionSeq (s:string) : seq<Direction> = s.Split(',') |> Seq.map ToDirection

let Test1Wire1 = "R8,U5,L5,D3"

let rec TracePath (p0:Point) (sd:seq<Direction>) : seq<Point> =
    let first = Seq.tryHead sd
    if first.IsNone then Seq.empty
    else 
        let t = (Trace p0 first.Value) |> Seq.toArray
        seq { yield! t
              yield! (TracePath (Array.last t) (Seq.tail sd) ) }

[<Theory>]
[<InlineData(3,3)>]
let TestWire1Has x y = 
    let tp : Point = {X=x; Y=y}
    let e = Seq.tryFind (fun p -> tp = p ) ( TracePath Point0 (ToDirectionSeq Test1Wire1) )
    Assert.True( e.IsSome )


