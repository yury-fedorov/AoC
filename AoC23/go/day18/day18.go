package day18

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"golang.org/x/exp/maps"
	"sort"
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

// Solution of the part 1.
func part1(m map[Point]rune) int {
	var part1 int
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
	return part1
}

// -- part 2 --

type Line struct {
	a, b Point
}

var Directions2 = map[rune]Point{
	'0': Right,
	'1': Down,
	'2': Left,
	'3': Up,
}

func orderedGrid(m2 []Line, getZ func(Point) int) []int {
	set := make(map[int]bool)
	for _, l := range m2 {
		set[getZ(l.a)] = true
		set[getZ(l.b)] = true
	}
	result := maps.Keys(set)
	sort.Ints(result)
	return result
}

// distance between close items
func diff(s []int) []int {
	s1 := s[1:]
	n := len(s1)
	result := make([]int, n)
	for i := 0; i < n; i++ {
		result[i] = s1[i] - s[i]
	}
	return result
}

func midGrid(grid []int) []int {
	d := diff(grid)
	var result []int
	for i, di := range d {
		result = append(result, grid[i]+(di/2))
	}
	return result
}

func isInRange(l Line, p Point, onX bool) bool {
	a := aoc.Ifelse(onX, l.a.x, l.a.y)
	b := aoc.Ifelse(onX, l.b.x, l.b.y)
	z := aoc.Ifelse(onX, p.x, p.y)
	return aoc.Min(a, b) <= z && z <= aoc.Max(a, b)
}

func isCrossed(l Line, p Point, d Point) bool {
	isLineHorizontal := l.a.y == l.b.y
	if isLineHorizontal && !isInRange(l, p, true) {
		// the horizontal line doesn't match the range [xa, xb] with point's x value
		return false
	}
	if !isLineHorizontal && !isInRange(l, p, false) {
		// the vertical line doesn't match the range [ya, yb] with point's y value
		return false
	}
	if d.y < 0 {
		return isLineHorizontal && l.a.y < p.y // up
	}
	if d.y > 0 {
		return isLineHorizontal && l.a.y > p.y // down
	}
	if isLineHorizontal {
		return false
	}
	if d.x < 0 && l.a.x < p.x {
		return true // left
	}
	if d.x > 0 && l.a.x > p.x {
		return true // right
	}
	return false
}

func foundCrossed(m2 []Line, p Point, d Point) []Line {
	var result []Line
	for _, l := range m2 {
		if isCrossed(l, p, d) {
			result = append(result, l)
		}
	}
	return result
}

func isIn2(m2 []Line, p Point) bool {
	for _, d := range Moves {
		crossed := foundCrossed(m2, p, d)
		if len(crossed)%2 != 1 {
			return false
		}
	}
	return true
}

func length(a, b int) int {
	return aoc.Max(a, b) - aoc.Min(a, b) + 1
}

func part2(m2 []Line) int {
	var part2 int
	xGrid := orderedGrid(m2, func(p Point) int { return p.x })
	yGrid := orderedGrid(m2, func(p Point) int { return p.y })
	midX := midGrid(xGrid)
	midY := midGrid(yGrid)
	isInCache := make(map[Point]bool) // x, y - indexes of grids
	for ix, px := range midX {
		for iy, py := range midY {
			in := isIn2(m2, Point{px, py})
			isInCache[Point{ix, iy}] = in
			if in {
				part2 += length(xGrid[ix], xGrid[ix+1]) * length(yGrid[iy], yGrid[iy+1])
			}
		}
	}
	// remove all adjacent in zone overlapping
	for ix, _ := range midX {
		for iy, _ := range midY {
			in := isInCache[Point{ix, iy}]
			// on the right could be overlapping
			inOnRight := isInCache[Point{ix + 1, iy}]
			if in && inOnRight {
				part2 -= length(yGrid[iy], yGrid[iy+1])
			}
			// on the bottom could be overlapping
			inOnBottom := isInCache[Point{ix, iy + 1}]
			if in && inOnBottom {
				part2 -= length(yGrid[ix], yGrid[ix+1])
			}
		}
	}

	return part2
}

func (day Day18) Solve() aoc.Solution {
	// Initialization requires different maps for part1 and part 2.
	var digPlan = aoc.ReadFile("18-1")
	m := make(map[Point]rune) // part 1 version fo the map
	var m2 []Line             // part 2 version of the map
	var p, p2 Point           // starting points
	m[p] = Dug
	for _, line := range digPlan {
		dq := strings.Split(line, " ")
		// part 1 map initialization
		d := Directions[dq[0]]
		q := aoc.Atoi(dq[1])
		for i := 0; i < q; i++ {
			p = move(p, d)
			m[p] = Dug
		}
		// part 2 map initialization
		code := dq[2]                             // (#489852)
		code = code[2 : len(code)-1]              // 489852
		d2code := []rune(code)[len(code)-1]       // last digit is direction code
		d2 := Directions2[d2code]                 // conversion to the direction
		code = code[:len(code)-1]                 // code without last digit: 489852 -> 48985
		q2, err := strconv.ParseInt(code, 16, 64) // from hex to int
		if err != nil {
			panic(err)
		}
		p2b := Point{x: p2.x + (d2.x * int(q2)), y: p2.y + (d2.y * int(q2))}
		m2 = append(m2, Line{p2, p2b})
		p2 = p2b
	}
	return aoc.Solution{strconv.Itoa(part1(m)), strconv.Itoa(part2(m2))}
}

// 952408144115 - right
// 1407387173222
// 952413211789
// 952408170675 (right now - closest)
// 952399900391 (removing + 1)
