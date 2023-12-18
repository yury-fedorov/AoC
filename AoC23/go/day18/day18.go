package day18

import (
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
var Moves = []Point{Up, Down, Left, Right}
var Directions = map[string]Point{
	"U": Up,
	"D": Down,
	"L": Left,
	"R": Right,
}

const Dug rune = '#'
const Outside rune = '!'

func move(p Point, d Point) Point {
	return Point{x: p.x + d.x, y: p.y + d.y}
}

func isIn(min Point, max Point, p Point) bool {
	return min.x <= p.x && max.x >= p.x && min.y <= p.y && max.y >= p.y
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
	var digPlan = aoc.ReadFile("18")
	m := make(map[Point]rune)
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

	var outside []Point
	for y := minP.y; y <= maxP.y; y++ {
		p := Point{minP.x, y}
		if m[p] != Dug {
			outside = append(outside, p)
		}
		p = Point{maxP.x, y}
		if m[p] != Dug {
			outside = append(outside, p)
		}
	}
	for x := minP.x; x <= maxP.x; x++ {
		p := Point{x, minP.y}
		if m[p] != Dug {
			outside = append(outside, p)
		}
		p = Point{x, maxP.y}
		if m[p] != Dug {
			outside = append(outside, p)
		}
	}

	done := make(map[Point]bool)
	for len(outside) > 0 {
		op := outside[0]
		outside = outside[1:]
		done[op] = true
		if m[op] == Outside {
			continue
		}
		m[op] = Outside
		for _, d := range Moves {
			op1 := move(op, d)
			if isIn(minP, maxP, op1) {
				if m[op1] != Dug && m[op1] != Outside {
					_, found := done[op1]
					if !found {
						outside = append(outside, op1)
					}
				}
			}
		}
	}
	for y := minP.y; y <= maxP.y; y++ {
		for x := minP.x; x <= maxP.x; x++ {
			ch := m[Point{x, y}]
			part1 += aoc.Ifelse(ch != Outside, 1, 0)
		}
	}

	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
