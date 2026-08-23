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

func (d Day17) Solve() aoc.Solution {
	var part1, part2 int
	p0 := Point{x: 0, y: 0}
	pq := &MinHeap{
		&QueueStep{position: p0, direction: Right, sumHeatLoss: 0, remainingStraightSteps: maxStepsStraight},
		&QueueStep{position: p0, direction: Down, sumHeatLoss: 0, remainingStraightSteps: maxStepsStraight},
	}
	heap.Init(pq)
	end := Point{x: xMax - 1, y: yMax - 1}

	// Key: position + direction + remainingStraightSteps
	// To properly track visited states, we need to consider all three factors
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
			part1 = nqs.sumHeatLoss
			break
		}

		p1List := nextMoves(pi, nqs.direction, nqs.remainingStraightSteps > 0)
		for _, p1i := range p1List {
			shl := nqs.sumHeatLoss + heatLossAt(p1i)
			newDir := direction(pi, p1i)
			remainingStraightSteps := aoc.Ifelse(nqs.direction == newDir, nqs.remainingStraightSteps, maxStepsStraight) - 1

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

	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}

func stateKeyFunc(p Point, d Point, remaining int) string {
	return strconv.Itoa(p.x) + "," + strconv.Itoa(p.y) + "," +
		strconv.Itoa(d.x) + "," + strconv.Itoa(d.y) + "," +
		strconv.Itoa(remaining)
}
