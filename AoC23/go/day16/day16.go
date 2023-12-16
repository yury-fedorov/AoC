package day16

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"golang.org/x/exp/slices" // Due to 1.20
	"strconv"
)

type Day16 struct{}

type Point struct {
	x, y int
}

var Right = Point{x: 1, y: 0}
var Left = Point{x: -1, y: 0}
var Up = Point{x: 0, y: -1}
var Down = Point{x: 0, y: 1}
var Directions = []Point{Right, Left, Up, Down}

func next(p0 Point, direction Point) Point {
	return Point{x: p0.x + direction.x, y: p0.y + direction.y}
}

func nextDirections(dir0 Point, material rune) []Point {
	sameDirection := []Point{dir0}
	isHorizontal := dir0.x != 0
	isVertical := !isHorizontal
	if material == '.' || (material == '-' && isHorizontal) || (material == '|' && isVertical) {
		return sameDirection
	}
	if isHorizontal && material == '|' {
		return []Point{Up, Down}
	}
	if isVertical && material == '-' {
		return []Point{Left, Right}
	}
	if material == '\\' {
		switch dir0 {
		case Right:
			return []Point{Down}
		case Left:
			return []Point{Up}
		case Up:
			return []Point{Left}
		case Down:
			return []Point{Right}
		}
	}
	if material == '/' {
		switch dir0 {
		case Right:
			return []Point{Up}
		case Left:
			return []Point{Down}
		case Up:
			return []Point{Right}
		case Down:
			return []Point{Left}
		}
	}
	// for all places where material didn't match or light
	return sameDirection
}

type DirectionQueueItem struct {
	startingPoint Point
	direction     Point
}

func isIn(p Point, xMax int, yMax int) bool {
	return !(p.x < 0 || p.y < 0 || p.x >= xMax || p.y >= yMax)
}

func inlight(m []string, p Point, d Point) int {
	var directionQueue []DirectionQueueItem
	var doneQueue []DirectionQueueItem
	xMax, yMax := len(m[0]), len(m)
	light := make(map[Point]bool)
	for {
		if !isIn(p, xMax, yMax) {
			if len(directionQueue) == 0 {
				break
			}
			ni := directionQueue[0]
			p = ni.startingPoint
			d = ni.direction
			directionQueue = directionQueue[1:]
		}
		light[p] = true
		material := []rune(m[p.y])[p.x]
		nd := nextDirections(d, material)
		if len(nd) == 1 {
			// single direction
			d = nd[0]
			p = next(p, d)
		} else {
			// multiple directions
			for _, di := range nd {
				pi := next(p, di)
				if isIn(pi, xMax, yMax) {
					dqi := DirectionQueueItem{startingPoint: pi, direction: di}
					if !slices.Contains(doneQueue, dqi) {
						directionQueue = append(directionQueue, dqi)
						doneQueue = append(doneQueue, dqi)
					}
				}
			}
			if len(directionQueue) == 0 {
				break
			}
			ni := directionQueue[0]
			p = ni.startingPoint
			d = ni.direction
			directionQueue = directionQueue[1:]
		}
	}
	return len(light)
}

func (day Day16) Solve() aoc.Solution {
	var part1, part2 int
	m := aoc.ReadFile("16")
	part1 = inlight(m, Point{x: 0, y: 0}, Right)

	part2 = part1
	xMax, yMax := len(m[0]), len(m)
	for x := 0; x < xMax; x++ {
		part2 = aoc.Max(part2, inlight(m, Point{x: x, y: 0}, Down))
		part2 = aoc.Max(part2, inlight(m, Point{x: x, y: yMax - 1}, Up))
	}
	for y := 0; y < yMax; y++ {
		part2 = aoc.Max(part2, inlight(m, Point{x: 0, y: y}, Right))
		part2 = aoc.Max(part2, inlight(m, Point{x: xMax - 1, y: y}, Left))
	}

	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
