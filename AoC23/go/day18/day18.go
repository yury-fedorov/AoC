package day18

import (
	"fmt"
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"golang.org/x/exp/maps"
	"strconv"
	"strings"
)

type Day18 struct{}

type Point struct {
	x, y int
}

var Up = Point{x: 0, y: -1}
var Down = Point{x: 0, y: 1}
var Left = Point{x: -1, y: 0}
var Right = Point{x: 1, y: 0}
var Directions = map[string]Point{
	"U": Up,
	"D": Down,
	"L": Left,
	"R": Right,
}

const Dug = true

func move(p Point, d Point) Point {
	return Point{x: p.x + d.x, y: p.y + d.y}
}

func minMax(points []Point) (min Point, max Point) {
	var minP, maxP Point
	minP = points[0]
	maxP = points[0]
	for _, p := range points {
		minP.x = aoc.Min(minP.x, p.x)
		minP.y = aoc.Min(minP.y, p.y)
		maxP.x = aoc.Max(maxP.x, p.x)
		maxP.y = aoc.Max(maxP.y, p.y)
	}
	return minP, maxP
}

func (day Day18) Solve() aoc.Solution {
	var part1, part2 int
	var digPlan = aoc.ReadFile("18-1")
	m := make(map[Point]bool)
	var p Point
	m[p] = Dug
	for _, line := range digPlan {
		dq := strings.Split(line, " ")
		d := Directions[dq[0]]
		q := aoc.Atoi(dq[1])
		for i := 0; i < q; i++ {
			p = move(p, d)
			m[p] = Dug
		}
	}

	minP, maxP := minMax(maps.Keys(m))
	for y := minP.y; y <= maxP.y; y++ {
		for x := minP.x; x <= maxP.x; x++ {
			fmt.Print(aoc.Ifelse(m[Point{x, y}], "#", "."))
		}
		fmt.Println()
	}
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
