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

let Manhattan (p: Point)  = abs(p.X)+abs(p.Y)

let Test1Wire2 = "U7,R6,D4,L4"

let Answer1 (p1:string) (p2:string) : int =
    let path1 = TracePath Point0 (ToDirectionSeq p1) |> Set.ofSeq
    let path2 = TracePath Point0 (ToDirectionSeq p2) |> Set.ofSeq
    let both = Set.intersect path1 path2
    both |> Seq.map Manhattan |> Seq.min

[<Theory>]
[<InlineData(1674)>]
let Part1 em =
    let lines = File.ReadAllLines("../../../Day03.txt")
    let a = lines.[0]
    let b = lines.[1]
    let m = Answer1 a b
    Assert.Equal( em, m )
    
