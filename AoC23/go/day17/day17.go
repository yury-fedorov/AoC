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

var m = aoc.ReadFile("17")
var xMax, yMax = len(m[0]), len(m)

func isIn(p Point) bool {
	return p.x >= 0 && p.y >= 0 && p.x < xMax && p.y < yMax
}

func nextDirs(dir0 Point, canGoStraight bool) []Point {
	i0 := slices.Index(Directions, dir0)
	dby := func(di int) Point {
		return Directions[(di+DirectionCount)%DirectionCount]
	}
	result := []Point{dir0, dby(i0 + 1), dby(i0 - 1)}
	return aoc.Ifelse(canGoStraight, result, result[1:])
}

func next(p0 Point, d Point) Point {
	return Point{x: p0.x + d.x, y: p0.y + d.y}
}

// returns points on the map (not directions)
func nextMoves(p0 Point, dir0 Point, canGoStraight bool) []Point {
	var result []Point
	for _, di := range nextDirs(dir0, canGoStraight) {
		p1 := next(p0, di)
		if isIn(p1) {
			result = append(result, p1)
		}
	}
	return result
}

type State struct {
	position         Point
	direction        Point
	maxStraightSteps int
}

func direction(p0, p1 Point) Point {
	return Point{x: p1.x - p0.x, y: p1.y - p0.y}
}

type QueueStep struct {
	position Point

	// data before the step
	direction              Point
	remainingStraightSteps int
	sumHeatLoss            int
}

func heatLossAt(p Point) int {
	return int([]rune(m[p.y])[p.x] - '0')
}

func (d Day17) Solve() aoc.Solution {
	var part1, part2 int
	p0 := Point{x: 0, y: 0}
	queue := []QueueStep{
		{position: p0, direction: Right, sumHeatLoss: 0, remainingStraightSteps: maxStepsStraight},
		{position: p0, direction: Down, sumHeatLoss: 0, remainingStraightSteps: maxStepsStraight},
	}
	end := Point{x: xMax - 1, y: yMax - 1}
	// total accumulated heat loss by the point
	minHeatLoss := map[Point]int{
		p0: 0,
	}
	for len(queue) > 0 {
		nqs := queue[0]
		queue = queue[1:]
		pi := nqs.position
		p1List := nextMoves(pi, nqs.direction, nqs.remainingStraightSteps > 0)
		for _, p1i := range p1List {
			shl := nqs.sumHeatLoss + heatLossAt(p1i)
			prev, found := minHeatLoss[p1i]
			isBestPath := false
			if !found || prev > shl {
				minHeatLoss[p1i] = shl
				isBestPath = true
			}
			if !isBestPath || p1i == end {
				continue
			}
			newDir := direction(pi, p1i)
			remainingStraightSteps := aoc.Ifelse(nqs.direction == newDir, nqs.remainingStraightSteps, maxStepsStraight) - 1
			queue = append(queue, QueueStep{position: p1i, direction: newDir, sumHeatLoss: shl,
				remainingStraightSteps: remainingStraightSteps})
		}
	}
	/*
		for y := 0; y < yMax; y++ {
			for x := 0; x < xMax; x++ {
				p := Point{x: x, y: y}
				shl := minHeatLoss[p]
				fmt.Print(aoc.Ifelse(shl <= 0, " ???", fmt.Sprintf(" %3d", shl)))
				fmt.Printf(" [%d] ", heatLossAt(p))
			}
			fmt.Println()
		}
	*/
	part1 = minHeatLoss[end]
	// 1254 - too low
	// 1281 ... 1285 - too high
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
