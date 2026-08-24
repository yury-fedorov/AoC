package day24

import (
	"math"
	"strconv"
	"strings"

	"github.com/yury-fedorov/AoC/AoC23/aoc"
)

type Day24 struct{}

type Point struct{ x, y, z float64 }

// 2D basics: https://www.cuemath.com/geometry/intersection-of-two-lines/
// Line in form: ax + by + c = 0 . Simplified in our case with assuming b=-1, to the form y = ax + c
type Line2DCoefficients struct{ a, b, c float64 }

// Input
type PointShift struct{ point, shift Point }

// Allow small floating-point tolerance
const eps = 1e-6

func equal(a, b float64) bool {
	return math.Abs(a-b) < eps
}

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

// Part 2: Find rock velocity by trying different values
// Returns (vx, vy, vz) or (0, 0, 0) if not found
func findRockVelocity(hailstones []PointShift) (float64, float64, float64) {
	// Try different velocities in a reasonable range
	for vx := -500.0; vx <= 500.0; vx++ {
		for vy := -500.0; vy <= 500.0; vy++ {
			for vz := -500.0; vz <= 500.0; vz++ {
				// Check if this velocity works for all hailstones
				if canHitAll(hailstones, vx, vy, vz) {
					return vx, vy, vz
				}
			}
		}
	}
	return 0, 0, 0
}

// Check if rock with given velocity can hit all hailstones
func canHitAll(hailstones []PointShift, rvx, rvy, rvz float64) bool {
	var rockPos Point
	var foundTime float64 = -1

	for i, h := range hailstones {
		// Transform to relative velocity frame
		// In rock's reference frame: hailstone velocity becomes (vxi - rvx, vyi - rvy, vzi - rvz)
		dvx := h.shift.x - rvx
		dvy := h.shift.y - rvy
		dvz := h.shift.z - rvz

		// If relative velocity is zero, rock moves with hailstone - no collision unless positions match
		if dvx == 0 && dvy == 0 && dvz == 0 {
			if i == 0 {
				rockPos = h.point
				foundTime = 0
			} else if rockPos.x != h.point.x || rockPos.y != h.point.y || rockPos.z != h.point.z {
				return false
			}
			continue
		}

		// Find collision time using first non-zero relative velocity component
		var t float64 = -1

		if dvx != 0 {
			t = -h.point.x / dvx
		} else if dvy != 0 {
			t = -h.point.y / dvy
		} else if dvz != 0 {
			t = -h.point.z / dvz
		}

		if t < 0 || (t != t) { // t != t checks for NaN
			return false
		}

		// Verify collision at time t
		hx := h.point.x + h.shift.x*t
		hy := h.point.y + h.shift.y*t
		hz := h.point.z + h.shift.z*t

		expectedRockX := rvx * t
		expectedRockY := rvy * t
		expectedRockZ := rvz * t

		if !(equal(hx, expectedRockX) && equal(hy, expectedRockY) && equal(hz, expectedRockZ)) {
			return false
		}

		// Calculate rock position at t=0
		if i == 0 {
			rockPos.x = h.point.x + h.shift.x*t - rvx*t
			rockPos.y = h.point.y + h.shift.y*t - rvy*t
			rockPos.z = h.point.z + h.shift.z*t - rvz*t
			foundTime = t
		} else {
			// Verify this collision is consistent with previous ones
			collideX := h.point.x + h.shift.x*t - rvx*t
			collideY := h.point.y + h.shift.y*t - rvy*t
			collideZ := h.point.z + h.shift.z*t - rvz*t

			if rockPos.x != collideX || rockPos.y != collideY || rockPos.z != collideZ {
				return false
			}
		}
	}

	return foundTime >= 0
}

func (day Day24) Solve() aoc.Solution {
	var part1, part2 int64
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
	for i := range n {
		for j := i + 1; j < n; j++ {
			psi := input[i]
			psj := input[j]
			p := intersectionPoint2D(toLineCoefficients(psi), toLineCoefficients(psj))
			if in(p.x) && in(p.y) && !isPast(p, psi) && !isPast(p, psj) {
				part1++
			}
		}
	}

	// Part 2: Find rock velocity and position
	rvx, rvy, rvz := findRockVelocity(input)

	// Calculate rock position at t=0
	if rvx != 0 || rvy != 0 || rvz != 0 {
		h := input[0]
		dvx := h.shift.x - rvx
		dvy := h.shift.y - rvy
		dvz := h.shift.z - rvz

		var t float64
		if dvx != 0 {
			t = -h.point.x / dvx
		} else if dvy != 0 {
			t = -h.point.y / dvy
		} else {
			t = -h.point.z / dvz
		}

		rockPosX := h.point.x + h.shift.x*t - rvx*t
		rockPosY := h.point.y + h.shift.y*t - rvy*t
		rockPosZ := h.point.z + h.shift.z*t - rvz*t

		part2 = int64(rockPosX + rockPosY + rockPosZ)
	}

	return aoc.Solution{Part1: strconv.FormatInt(part1, 10), Part2: strconv.FormatInt(part2, 10)}
	// part2 613391294577878 -- too low
}

/* alternative version to explore

import (
	"math/big"
)

type Vector struct {
	x, y, z *big.Rat
}

type Hailstone struct {
	p, v Vector
}

func solvePart2(hailstones []Hailstone) int64 {
	// Pick 3 hailstones that are non-parallel
	h0, h1, h2 := hailstones[0], hailstones[1], hailstones[2]

	// Build 6x6 matrix A and 6x1 vector B for A * X = B
	// Unknowns X = [px, py, pz, vx, vy, vz]
	A := make([][]*big.Rat, 6)
	B := make([]*big.Rat, 6)
	for i := range A {
		A[i] = make([]*big.Rat, 6)
		for j := range A[i] {
			A[i][j] = big.NewRat(0, 1)
		}
		B[i] = big.NewRat(0, 1)
	}

	// Helper to add equations from pair (hA, hB)
	addPair := func(rowOffset int, hA, hB Hailstone) {
		// (vA - vB) x p_r + (pB - pA) x v_r = pB x vB - pA x vA
		// Cross product expanded into 3 scalar equations (X, Y, Z components)

		// Row 0 (X): (vAy - vBy)*pz - (vAz - vBz)*py + (pBz - pAz)*vy - (pBy - pAy)*vz
		// ... maps coefficients to [px, py, pz, vx, vy, vz]

		// Equation for X component
		A[rowOffset+0][1] = sub(hA.v.z, hB.v.z) // py
		A[rowOffset+0][2] = sub(hB.v.y, hA.v.y) // pz
		A[rowOffset+0][4] = sub(hB.p.z, hA.p.z) // vy
		A[rowOffset+0][5] = sub(hA.p.y, hB.p.y) // vz
		B[rowOffset+0] = sub(
			sub(mul(hB.p.y, hB.v.z), mul(hB.p.z, hB.v.y)),
			sub(mul(hA.p.y, hA.v.z), mul(hA.p.z, hA.v.y)),
		)

		// Equation for Y component
		A[rowOffset+1][0] = sub(hB.v.z, hA.v.z) // px
		A[rowOffset+1][2] = sub(hA.v.x, hB.v.x) // pz
		A[rowOffset+1][3] = sub(hA.p.z, hB.p.z) // vx
		A[rowOffset+1][5] = sub(hB.p.x, hA.p.x) // vz
		B[rowOffset+1] = sub(
			sub(mul(hB.p.z, hB.v.x), mul(hB.p.x, hB.v.z)),
			sub(mul(hA.p.z, hA.v.x), mul(hA.p.x, hA.v.z)),
		)

		// Equation for Z component
		A[rowOffset+2][0] = sub(hA.v.y, hB.v.y) // px
		A[rowOffset+2][1] = sub(hB.v.x, hA.v.x) // py
		A[rowOffset+2][3] = sub(hB.p.y, hA.p.y) // vx
		A[rowOffset+2][4] = sub(hA.p.x, hB.p.x) // vy
		B[rowOffset+2] = sub(
			sub(mul(hB.p.x, hB.v.y), mul(hB.p.y, hB.v.x)),
			sub(mul(hA.p.x, hA.v.y), mul(hA.p.y, hA.v.x)),
		)
	}

	addPair(0, h0, h1)
	addPair(3, h0, h2)

	// Solve system A * X = B via Gaussian Elimination
	X := gaussianElimination(A, B)

	// Sum px + py + pz (indices 0, 1, 2)
	sum := new(big.Rat).Add(X[0], X[1])
	sum.Add(sum, X[2])

	res, _ := sum.Num().Quo(sum.Num(), sum.Denom()).Int64()
	return res
}

// Helper arithmetic functions for big.Rat
func add(a, b *big.Rat) *big.Rat { return new(big.Rat).Add(a, b) }
func sub(a, b *big.Rat) *big.Rat { return new(big.Rat).Sub(a, b) }
func mul(a, b *big.Rat) *big.Rat { return new(big.Rat).Mul(a, b) }
func div(a, b *big.Rat) *big.Rat { return new(big.Rat).Quo(a, b) }

func gaussianElimination(A [][]*big.Rat, B []*big.Rat) []*big.Rat {
	n := len(B)
	for i := 0; i < n; i++ {
		// Pivot
		pivot := A[i][i]
		for j := i; j < n; j++ {
			A[i][j] = div(A[i][j], pivot)
		}
		B[i] = div(B[i], pivot)

		for k := 0; k < n; k++ {
			if k != i {
				factor := A[k][i]
				for j := i; j < n; j++ {
					A[k][j] = sub(A[k][j], mul(factor, A[i][j]))
				}
				B[k] = sub(B[k], mul(factor, B[i]))
			}
		}
	}
	return B
}

*/
