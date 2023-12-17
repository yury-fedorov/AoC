package day17

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"golang.org/x/exp/slices" // Due to 1.20
	"strconv"
)

type Day17 struct{}

type Point struct {
	x, y int
}

const maxStepsStraight int = 3

var Right = Point{x: 1, y: 0}
var Left = Point{x: -1, y: 0}
var Up = Point{x: 0, y: -1}
var Down = Point{x: 0, y: 1}
var Directions = []Point{Right, Down, Left, Up}
var DirectionCount = len(Directions)

var m = aoc.ReadFile("17-1")
var xMax, yMax = len(m[0]), len(m)

func isIn(p Point) bool {
	return p.x >= 0 && p.y >= 0 && p.x < xMax && p.y < yMax
}

func nextDirs(dir0 Point) []Point {
	i0 := slices.Index(Directions, dir0)
	dby := func(di int) Point {
		if di < 0 {
			di += DirectionCount
		}
		return Directions[di%DirectionCount]
	}
	return []Point{dir0, dby(i0 + 1), dby(i0 - 1)}
}

func next(p0 Point, d Point) Point {
	return Point{x: p0.x + d.x, y: p0.y + d.y}
}

// returns points on the map (not directions)
func nextMoves(p0 Point, dir0 Point) []Point {
	var result []Point
	for _, di := range nextDirs(dir0) {
		p1 := next(p0, di)
		if isIn(p1) {
			result = append(result, p1)
		}
	}
	return result
}

func (d Day17) Solve() aoc.Solution {
	var part1, part2 int
	// for _, line := range  {
	// }
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
