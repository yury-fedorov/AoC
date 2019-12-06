module Day06

open System
open System.IO
open Xunit
 
type Tree = 
    | Node of Value:string * Parent:Tree
    | None

let sampleFileName = "../../../Day06Sample.txt"

let fileName = "../../../Day06.txt"

let RootNode = Node(Value = "COM", Parent = None) 

let Init file =
    let lines = File.ReadAllLines file |> Array.map (fun line -> line.Split ')')
    let first = lines |> Array.find (fun a -> a.[0] = RootNode.Value)
    let mutable map = [ RootNode.Value, RootNode ] |> Map.ofList
    let parent = RootNode
    let small = first.[1]
    let node = Node(Value=small, Parent = parent)
    map <- map.Add( small, node )



