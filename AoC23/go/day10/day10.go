package day10

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"slices"
	"strconv"
)

type Day10 struct{}

type Point struct {
	x, y int
}

type Tile rune

var Directions = []Point{{x: 1, y: 0}, {x: -1, y: 0}, {x: 0, y: -1}, {x: 0, y: 1}}

const (
	VertPipe Tile = '|'
	HorPipe       = '-'
	NEBend        = 'L'
	NWBend        = 'J'
	SWBend        = '7'
	SEBend        = 'F'
	Ground        = '.'
	Start         = 'S'
)

var South = Point{x: 0, y: 1}
var North = Point{x: 0, y: -1}
var West = Point{x: -1, y: 0}
var East = Point{x: 1, y: 0}

var Moves = []Point{South, North, West, East}

var Shifts = map[Tile][]Point{
	VertPipe: {North, South},
	HorPipe:  {West, East},
	NEBend:   {North, East},
	NWBend:   {North, West},
	SWBend:   {South, West},
	SEBend:   {South, East},
}

func parse(file string) [][]Tile {
	var result [][]Tile
	for _, line := range aoc.ReadFile(file) {
		row := []Tile(line)
		result = append(result, row)
	}
	return result
}

func xy(loop [][]Tile) (int, int) {
	yMax := len(loop)
	xMax := len(loop[0])
	return xMax, yMax
}

func at(loop [][]Tile, x int, y int) Tile {
	if x < 0 || y < 0 {
		return Ground
	}
	xMax, yMax := xy(loop)
	if x >= xMax || y >= yMax {
		return Ground
	}
	return loop[y][x]
}

func isDir(directions []Point, dir Point) bool {
	return slices.Contains(directions, dir)
}

func opposite(p Point) Point {
	return Point{x: -p.x, y: -p.y}
}

func start(loop [][]Tile) Point {
	xMax, yMax := xy(loop)
	for y := 0; y < yMax; y++ {
		for x := 0; x < xMax; x++ {
			if at(loop, x, y) == Start {
				return Point{x: x, y: y}
			}
		}
	}
	panic("")
}

func nextDir(loop [][]Tile, start Point) []Point {
	var result []Point
	for _, dir := range Moves { // ie. North
		from := opposite(dir) // North -> South
		dirTile := at(loop, start.x+dir.x, start.y+dir.y)
		for tile, dirs := range Shifts {
			if tile == dirTile && isDir(dirs, from) {
				result = append(result, dir)
				break
			}
		}
	}
	return result
}

func toTile(dirs []Point) Tile {
	for tile, shiftDirs := range Shifts {
		found := true
		for _, sd := range shiftDirs {
			if !isDir(dirs, sd) {
				found = false
				break
			}
		}
		if found {
			return tile
		}
	}
	panic("")
}

type NextStep struct {
	point          Point
	stepsTillPoint int
}

func move(start Point, d Point) Point {
	return Point{x: start.x + d.x, y: start.y + d.y}
}

func (d Day10) Solve() aoc.Solution {
	var part1, part2 int
	loop := parse("10")
	startPoint := start(loop)
	nextDirs := nextDir(loop, startPoint)
	startTile := toTile(nextDirs)
	// fixing the map
	loop[startPoint.y][startPoint.x] = startTile

	path := make(map[Point]int)
	path[startPoint] = 0
	var queue []NextStep
	for _, nd := range nextDirs {
		queue = append(queue, NextStep{point: move(startPoint, nd), stepsTillPoint: 1})
	}

	for len(queue) > 0 {
		ns := queue[0]
		queue = queue[1:]

		nextDirs = Shifts[at(loop, ns.point.x, ns.point.y)]
		for _, nd := range nextDirs {
			p := move(ns.point, nd)
			_, found := path[p]
			if found {
				continue
			}
			steps := ns.stepsTillPoint + 1
			path[p] = steps
			queue = append(queue, NextStep{point: p, stepsTillPoint: steps})
		}
	}

	part1 = len(path) / 2

	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
