package day24

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"strconv"
	"strings"
)

type Day24 struct{}

type Point struct{ x, y, z float64 }

// 2D basics: https://www.cuemath.com/geometry/intersection-of-two-lines/
// Line in form: ax + by + c = 0 . Simplified in our case with assuming b=-1, to the form y = ax + c
type Line2DCoefficients struct{ a, b, c float64 }

// Input
type PointShift struct{ point, shift Point }

func toLineCoefficients(ps PointShift) Line2DCoefficients {
	x0 := ps.point.x
	y0 := ps.point.y
	dx := ps.shift.x
	dy := ps.shift.y
	// assume ax + by + c = 0 and b = -1 => y = ax + c
	// y0 - ax0 = y1 - ax1 => a = dy/dx
	a := dy / dx
	// c = y0 - ax0
	c := y0 - (a * x0)
	return Line2DCoefficients{a: a, b: -1.0, c: c}
}

func intersectionPoint2D(one, two Line2DCoefficients) Point {
	x := (two.c - one.c) / (one.a - two.a)
	y := one.a*x + one.c
	return Point{x: x, y: y, z: 0.0}
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
	input := read("24") // sample - 5 element, my input - 300 elements
	// for sample: in := func(v float64) bool { return v >= 7.0 && v <= 27 }
	in := func(v float64) bool { return v >= 200000000000000 && v <= 400000000000000 }
	isPast := func(p Point, ps PointShift) bool {
		x := func(p Point) float64 { return p.x }
		y := func(p Point) float64 { return p.y }
		past := func(p Point, ps PointShift, v func(p Point) float64) bool {
			px := v(p)
			p0 := v(ps.point)
			sx := v(ps.shift)
			if p0 == px {
				return false
			}
			return (sx > 0 && p0 > px) || (sx < 0 && p0 < px)
		}
		return past(p, ps, x) || past(p, ps, y)
	}
	n := len(input)
	for i := 0; i < n; i++ {
		for j := i + 1; j < n; j++ {
			psi := input[i]
			psj := input[j]
			p := intersectionPoint2D(toLineCoefficients(psi), toLineCoefficients(psj))
			if in(p.x) && in(p.y) && !isPast(p, psi) && !isPast(p, psj) {
				part1++
			}
		}
	}
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
