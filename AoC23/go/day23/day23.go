package day23

import (
	"slices"
	"strconv"
	"strings"

	"github.com/yury-fedorov/AoC/AoC23/aoc"
)

type Day23 struct{}

type Point struct{ x, y int16 }

var shifts = []Point{{0, 1}, {0, -1}, {1, 0}, {-1, 0}}

const Forrest = '#'
const Path = '.'
const Slopes = "v^><" // same sequence as shifts

var m = aoc.ReadFile("23")
var x0 = int16(slices.Index([]rune(m[0]), Path))
var yMax = int16(len(m) - 1)
var xMax = int16(len(m[0]) - 1)
var xEnd = int16(slices.Index([]rune(m[yMax]), Path))
var pStart = Point{x0, 0}
var pEnd = Point{xEnd, yMax}

func at(p Point) rune {
	if p.x < 0 || p.y < 0 || p.y > yMax || p.x > xMax {
		return Forrest
	}
	return ([]rune(m[p.y]))[p.x]
}

func checkSlope(material rune) (bool, Point) {
	i := strings.Index(Slopes, string(material))
	isSlope := i >= 0
	p := Point{}
	if isSlope {
		p = shifts[i]
	}
	return isSlope, p
}

func shift(p, s Point) Point { return Point{x: p.x + s.x, y: p.y + s.y} }

type CacheNextKey struct {
	point Point
	part  aoc.Part
}

var cacheNext = make(map[CacheNextKey][]Point)

func next(p0 Point, part aoc.Part) []Point {
	key := CacheNextKey{point: p0, part: part}
	v, ok := cacheNext[key]
	if ok {
		return v
	}

	material := at(p0)
	if part == aoc.Part1 {
		isSlope, slopeShift := checkSlope(material)
		if isSlope {
			return []Point{shift(p0, slopeShift)}
		}
	}
	var result []Point
	for _, s := range shifts {
		p1 := shift(p0, s)
		material = at(p1)
		if material != Forrest {
			result = append(result, p1)
		}
	}
	cacheNext[key] = result
	return result
}

func paths(path0 []Point, part aoc.Part) int {
	n := len(path0)
	last := path0[n-1]
	nn := next(last, part)
	twoBack := Point{}
	if n >= 2 {
		twoBack = path0[n-2]
	}
	var result int
	for _, n := range nn {
		if n == twoBack || slices.Contains(path0, n) {
			continue
		}
		if n == pEnd {
			return len(path0)
		}
		result = max(result, paths(append(path0, n), part))
	}
	return result
}

// -- part 2: contract corridors into a weighted junction graph --
//
// The raw-grid DFS is exponential over every cell, including long straight
// corridors. Contracting each corridor (straight runs and corners) into a
// single weighted edge collapses the grid into a small graph of junctions
// (branch points + start/end), making the longest-path search tractable.

// A junction is any walkable cell whose walkable-neighbor count is not 2,
// plus the start and end. Corridor cells (degree 2) are contracted away.
func isJunction(p Point) bool {
	if p == pStart || p == pEnd {
		return true
	}
	deg := 0
	for _, s := range shifts {
		if at(shift(p, s)) != Forrest {
			deg++
		}
	}
	return deg != 2
}

type edge struct {
	to     int
	weight int
}

// walk follows the corridor from junction `from` in direction `dir` until it
// reaches another junction, returning that junction and the step count.
// Slopes are ignored (Part 2 rules: every cell is walkable both ways).
func walk(from Point, dir Point) (Point, int) {
	prev := from
	cur := shift(from, dir)
	steps := 1
	for !isJunction(cur) {
		var nxt Point
		for _, s := range shifts {
			cand := shift(cur, s)
			if cand == prev {
				continue
			}
			if at(cand) != Forrest {
				nxt = cand
				break
			}
		}
		prev = cur
		cur = nxt
		steps++
	}
	return cur, steps
}

func buildJunctionGraph() map[int][]edge {
	indexOf := make(map[Point]int)
	var junctions []Point
	add := func(p Point) {
		if _, ok := indexOf[p]; !ok {
			indexOf[p] = len(junctions)
			junctions = append(junctions, p)
		}
	}
	add(pStart)
	add(pEnd)
	for y := int16(0); y <= yMax; y++ {
		for x := int16(0); x <= xMax; x++ {
			p := Point{x, y}
			if at(p) != Forrest && isJunction(p) {
				add(p)
			}
		}
	}
	adj := make(map[int][]edge)
	for _, jp := range junctions {
		from := indexOf[jp]
		for _, s := range shifts {
			nb := shift(jp, s)
			if at(nb) == Forrest {
				continue
			}
			dest, weight := walk(jp, s)
			adj[from] = append(adj[from], edge{to: indexOf[dest], weight: weight})
		}
	}
	return adj
}

func longestPath(adj map[int][]edge, startIdx, endIdx int) int {
	visited := make(map[int]bool)
	var dfs func(int) int
	dfs = func(node int) int {
		if node == endIdx {
			return 0
		}
		visited[node] = true
		best := -1
		for _, e := range adj[node] {
			if visited[e.to] {
				continue
			}
			if sub := dfs(e.to); sub >= 0 {
				best = max(best, e.weight+sub)
			}
		}
		visited[node] = false
		return best
	}
	return dfs(startIdx)
}

func solutionPart2() int {
	adj := buildJunctionGraph()
	return longestPath(adj, 0, 1) // 0 = pStart, 1 = pEnd (added first)
}

func solution(part aoc.Part) int { return paths([]Point{pStart}, part) }

func (day Day23) Solve() aoc.Solution {
	return aoc.Solution{
		Part1: strconv.Itoa(solution(aoc.Part1)),
		Part2: strconv.Itoa(solutionPart2()),
	}
}
