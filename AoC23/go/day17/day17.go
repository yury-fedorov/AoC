package day17

import (
	"container/heap"
	"slices"
	"strconv"

	"github.com/yury-fedorov/AoC/AoC23/aoc"
)

type Day17 struct{}

type Point struct {
	x, y int
}

const maxStepsStraightPart1 int = 3
const maxStepsStraightPart2 int = 10
const minStepsStraightPart2 int = 4

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

func nextDirs(dir0 Point, canGoStraight bool, canTurn bool) []Point {
	i0 := slices.Index(Directions, dir0)
	dby := func(di int) Point {
		return Directions[(di+DirectionCount)%DirectionCount]
	}
	if canGoStraight && !canTurn {
		return []Point{dir0}
	}
	if !canGoStraight && canTurn {
		return []Point{dby(i0 + 1), dby(i0 - 1)}
	}
	return []Point{dir0, dby(i0 + 1), dby(i0 - 1)}
}

func next(p0 Point, d Point) Point {
	return Point{x: p0.x + d.x, y: p0.y + d.y}
}

// returns points on the map (not directions)
func nextMoves(p0 Point, dir0 Point, canGoStraight bool, canTurn bool) []Point {
	var result []Point
	for _, di := range nextDirs(dir0, canGoStraight, canTurn) {
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
	index                  int // for heap.Interface
}

// MinHeap implements heap.Interface for QueueStep
type MinHeap []*QueueStep

func (h MinHeap) Len() int           { return len(h) }
func (h MinHeap) Less(i, j int) bool { return h[i].sumHeatLoss < h[j].sumHeatLoss }
func (h MinHeap) Swap(i, j int) {
	h[i], h[j] = h[j], h[i]
	h[i].index = i
	h[j].index = j
}

func (h *MinHeap) Push(x interface{}) {
	item := x.(*QueueStep)
	item.index = len(*h)
	*h = append(*h, item)
}

func (h *MinHeap) Pop() interface{} {
	old := *h
	n := len(old)
	item := old[n-1]
	old[n-1] = nil
	item.index = -1
	*h = old[0 : n-1]
	return item
}

func heatLossAt(p Point) int {
	return int([]rune(m[p.y])[p.x] - '0')
}

func solvePart(maxSteps, minSteps int) int {
	p0 := Point{x: 0, y: 0}
	pq := &MinHeap{
		&QueueStep{position: p0, direction: Right, sumHeatLoss: 0, remainingStraightSteps: maxSteps},
		&QueueStep{position: p0, direction: Down, sumHeatLoss: 0, remainingStraightSteps: maxSteps},
	}
	heap.Init(pq)
	end := Point{x: xMax - 1, y: yMax - 1}

	// Key: position + direction + remainingStraightSteps
	visited := make(map[string]bool)

	for pq.Len() > 0 {
		nqs := heap.Pop(pq).(*QueueStep)
		pi := nqs.position

		// Create a state key to track if we've already processed this exact state
		stateKey := stateKeyFunc(pi, nqs.direction, nqs.remainingStraightSteps)
		if visited[stateKey] {
			continue
		}
		visited[stateKey] = true

		// If we reached the end, we have the answer (first time is optimal due to priority queue)
		if pi == end {
			return nqs.sumHeatLoss
		}
		canTurn := nqs.remainingStraightSteps <= maxSteps-minSteps
		nextDirsList := nextMoves(pi, nqs.direction, nqs.remainingStraightSteps > 0, canTurn)
		for _, p1i := range nextDirsList {
			shl := nqs.sumHeatLoss + heatLossAt(p1i)
			newDir := direction(pi, p1i)
			remainingStraightSteps := aoc.Ifelse(nqs.direction == newDir, nqs.remainingStraightSteps, maxSteps) - 1

			stateKey := stateKeyFunc(p1i, newDir, remainingStraightSteps)
			if !visited[stateKey] {
				heap.Push(pq, &QueueStep{
					position:               p1i,
					direction:              newDir,
					sumHeatLoss:            shl,
					remainingStraightSteps: remainingStraightSteps,
				})
			}
		}
	}

	return -1 // Should not reach here if input is valid
}

func (d Day17) Solve() aoc.Solution {
	part1 := solvePart(maxStepsStraightPart1, 1)
	part2 := solvePart(maxStepsStraightPart2, minStepsStraightPart2)
	return aoc.Solution{Part1: strconv.Itoa(part1), Part2: strconv.Itoa(part2)}
}

func stateKeyFunc(p Point, d Point, remaining int) string {
	return strconv.Itoa(p.x) + "," + strconv.Itoa(p.y) + "," +
		strconv.Itoa(d.x) + "," + strconv.Itoa(d.y) + "," +
		strconv.Itoa(remaining)
}
