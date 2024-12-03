package day23

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"golang.org/x/exp/slices"
	"strconv"
	"strings"
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
	twoBack := Point{} // path0 greater than 1 element is dominant
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
		result = aoc.Max(result, paths(append(path0, n), part))
	}
	return result
}

func solution(part aoc.Part) int { return paths([]Point{pStart}, part) }

func (day Day23) Solve() aoc.Solution {
	return aoc.Solution{strconv.Itoa(solution(aoc.Part1)), strconv.Itoa(solution(aoc.Part2))}
}
