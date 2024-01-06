package day24

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"strconv"
	"strings"
)

type Day24 struct{}

type Point struct{ x, y, z float64 }

// 2D basics: https://www.cuemath.com/geometry/intersection-of-two-lines/
// Line in form: ax + by + cz + d = 0
type LineCoefficients struct{ a, b, c, d float64 }

// Input
type PointShift struct{ point, shift Point }

func toLineCoefficients(ps PointShift) LineCoefficients {
	return LineCoefficients{} // TODO implement
}

func intersectionPoint(one, two LineCoefficients) Point {
	base := (one.a * two.b) - (two.a * one.b) // a1 * b2 - a2 * b1
	x := ((one.b * two.d) - (two.b - one.d)) / base
	y := ((one.d * two.a) - (two.d - one.a)) / base
	z := 0.0 // TODO - add z dimension in the calculation
	return Point{x: x, y: y, z: z}
}

func parse(line string) PointShift {
	// 19, 13, 30 @ -2, 1, -2
	pp := strings.Split(line, " @ ")
	f := func(p string) Point {
		n := func(p int64) float64 { return float64(p) }
		t := strings.ReplaceAll(p, ", ", " ")
		t = strings.ReplaceAll(t, "  ", " ")
		t = strings.Trim(t, " ")
		a := aoc.ToSlice(t)
		return Point{x: n(a[0]), y: n(a[1]), z: n(a[2])}
	}
	return PointShift{point: f(pp[0]), shift: f(pp[1])}
}

func read(file string) []PointShift {
	var result []PointShift
	for _, line := range aoc.ReadFile(file) {
		result = append(result, parse(line))
	}
	return result
}

func (day Day24) Solve() aoc.Solution {
	var part1, part2 int

	// sample 1
	/* TODO:
	Hailstone A: 19, 13, 30 @ -2, 1, -2
	Hailstone B: 18, 19, 22 @ -1, -1, -2
	Hailstones' paths will cross inside the test area (at x=14.333, y=15.333)
	*/

	input := read("24-1") // sample - 5 element, my input - 300 elements
	part1 = 0 * len(input)
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
