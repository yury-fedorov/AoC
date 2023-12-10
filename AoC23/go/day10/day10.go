package day10

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"math"
	"slices"
	"strconv"
)

type Day10 struct{}

type Point struct {
	x, y int
}

type Tile rune

const (
	VertPipe    Tile = '|'
	HorPipe     Tile = '-'
	NEBend      Tile = 'L'
	NWBend      Tile = 'J'
	SWBend      Tile = '7'
	SEBend      Tile = 'F'
	Ground      Tile = '.'
	Start       Tile = 'S'
	OuterGround Tile = 'O' // for part 2
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
		return OuterGround
	}
	xMax, yMax := xy(loop)
	if x >= xMax || y >= yMax {
		return OuterGround
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

func countCrossedBoarders(loop [][]Tile, path map[Point]int, p Point, d Point) int {
	var result int
	var s []int
	var tiles []Tile
	for pi := p; true; pi = move(pi, d) {
		t := at(loop, pi.x, pi.y)
		steps, isPipeline := path[pi]
		if isPipeline {
			s = append(s, steps)
			tiles = append(tiles, t)
		} else {
			// not pipeline anymore
			if len(s) == 0 {
			} else if len(s) == 1 {
				result += 1
			} else {
				s0 := s[0]
				s = s[1:]
				for len(s) > 0 {
					s1 := s[0]
					s = s[1:]
					if math.Abs(float64(s1-s0)) > 1.0 {
						result++
					}
					s0 = s1
				}
				result++
			}
			s = []int{}
			tiles = []Tile{}

		}
		if t == OuterGround {
			break
		}
	}
	return result
}

func isInternal(loop [][]Tile, path map[Point]int, p Point) bool {
	for _, d := range Moves {
		c := countCrossedBoarders(loop, path, p, d)
		if c%2 != 1 {
			return false
		}
	}
	return true
}

func (d Day10) Solve() aoc.Solution {
	var part1, part2 int
	loop := parse("10-6")
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

	// part 2
	// clean from all not necessary tubes
	candidates := make(map[Point]bool)
	xMax, yMax := xy(loop)
	for y := 0; y < yMax; y++ {
		for x := 0; x < xMax; x++ {
			p := Point{x: x, y: y}
			_, found := path[p]
			if !found {
				// candidates
				// all candidates at the boarders are outer
				border := x == 0 || y == 0 || (x+1) == xMax || (y+1) == yMax
				loop[y][x] = aoc.Ifelse(border, OuterGround, Ground)
				candidates[p] = !border
			}
		}
	}

	for p, isCandidate := range candidates {
		part2 += aoc.Ifelse(isCandidate && isInternal(loop, path, p), 1, 0)
	}

	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
