module Day06

open System
open System.IO
open Xunit
 
type Tree = 
    | Node of Name:string * Parent:Tree
    | None

let sampleFileName = "../../../Day06Sample.txt"

let fileName = "../../../Day06.txt"

let RootName = "COM"

let RootNode = Node(Name = RootName, Parent = None) 

let rec Init (lines:seq<array<string>>) (map:Map<string,Node>) (parent:Node)  =
    // could be more than one
    let mutable localMap = map
    let children = lines |> Seq.filter (fun a -> a.[0] = parent.Name ) |> Seq.map (fun a -> a.[1])
    for childName in children do
        let childNode = Node(Name=childName, Parent = parent)
        localMap <- localMap.Add( childName, childNode ) 

    // how we remove a line from the array or list?
    let mutable other = lines |> Seq.filter (fun a -> a.[0] <> parent.Name)
    if Seq.isEmpty other then (other, localMap) 
    else 
        for childName in children do 
            let childNode = localMap.[childName]
            let (map1, list1) = Init other localMap childNode 

let Init file =
    let lines = File.ReadAllLines file |> Array.map (fun line -> line.Split ')')
    let first = lines |> Array.find (fun a -> a.[0] = RootName)
    let mutable map = [ RootName, RootNode ] |> Map.ofList
    let parent = RootNode
    let small = first.[1]
    let node = Node(Value=small, Parent = parent)
    map <- map.Add( small, node )
    // how we remove a line from the array or list?
    



