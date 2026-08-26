package day24

import (
	"math/big"
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

type VectorRat struct {
	x, y, z *big.Rat
}

type HailstoneRat struct {
	p, v VectorRat
}

func toRat(f float64) *big.Rat {
	return big.NewRat(int64(f), 1)
}

func toHailstoneRat(ps PointShift) HailstoneRat {
	return HailstoneRat{
		p: VectorRat{x: toRat(ps.point.x), y: toRat(ps.point.y), z: toRat(ps.point.z)},
		v: VectorRat{x: toRat(ps.shift.x), y: toRat(ps.shift.y), z: toRat(ps.shift.z)},
	}
}

func addRat(a, b *big.Rat) *big.Rat { return new(big.Rat).Add(a, b) }
func subRat(a, b *big.Rat) *big.Rat { return new(big.Rat).Sub(a, b) }
func mulRat(a, b *big.Rat) *big.Rat { return new(big.Rat).Mul(a, b) }
func divRat(a, b *big.Rat) *big.Rat { return new(big.Rat).Quo(a, b) }
func negRat(a *big.Rat) *big.Rat    { return new(big.Rat).Neg(a) }

// solvePart2 finds the sum of initial (x, y, z) coordinates of the rock.
// For any hailstone i, (p_r - p_i) x (v_r - v_i) = 0
// Expanding: p_r x v_r = p_r x v_i + p_i x v_r - p_i x v_i
// Equating for hailstones i and j eliminates the non-linear p_r x v_r term:
// p_r x (v_i - v_j) + (p_i - p_j) x v_r = p_i x v_i - p_j x v_j
// With 3 hailstones (2 pairs), we get a 6x6 linear system for (p_rx, p_ry, p_rz, v_rx, v_ry, v_rz).
func solvePart2(hailstones []PointShift) int64 {
	h0 := toHailstoneRat(hailstones[0])
	h1 := toHailstoneRat(hailstones[1])
	h2 := toHailstoneRat(hailstones[2])

	A := make([][]*big.Rat, 6)
	B := make([]*big.Rat, 6)
	for i := range A {
		A[i] = make([]*big.Rat, 6)
		for j := range A[i] {
			A[i][j] = big.NewRat(0, 1)
		}
		B[i] = big.NewRat(0, 1)
	}

	addPair := func(rowOffset int, hA, hB HailstoneRat) {
		// Delta v = vA - vB
		dvx := subRat(hA.v.x, hB.v.x)
		dvy := subRat(hA.v.y, hB.v.y)
		dvz := subRat(hA.v.z, hB.v.z)

		// Delta p = pA - pB
		dpx := subRat(hA.p.x, hB.p.x)
		dpy := subRat(hA.p.y, hB.p.y)
		dpz := subRat(hA.p.z, hB.p.z)

		// Cross products pA x vA and pB x vB
		cAx := subRat(mulRat(hA.p.y, hA.v.z), mulRat(hA.p.z, hA.v.y))
		cAy := subRat(mulRat(hA.p.z, hA.v.x), mulRat(hA.p.x, hA.v.z))
		cAz := subRat(mulRat(hA.p.x, hA.v.y), mulRat(hA.p.y, hA.v.x))

		cBx := subRat(mulRat(hB.p.y, hB.v.z), mulRat(hB.p.z, hB.v.y))
		cBy := subRat(mulRat(hB.p.z, hB.v.x), mulRat(hB.p.x, hB.v.z))
		cBz := subRat(mulRat(hB.p.x, hB.v.y), mulRat(hB.p.y, hB.v.x))

		// Row 0: X-component
		// 0*px + (dvz)*py + (-dvy)*pz + 0*vx + (-dpz)*vy + (dpy)*vz = cAx - cBx
		A[rowOffset+0][0] = big.NewRat(0, 1)
		A[rowOffset+0][1] = dvz
		A[rowOffset+0][2] = negRat(dvy)
		A[rowOffset+0][3] = big.NewRat(0, 1)
		A[rowOffset+0][4] = negRat(dpz)
		A[rowOffset+0][5] = dpy
		B[rowOffset+0] = subRat(cAx, cBx)

		// Row 1: Y-component
		// (-dvz)*px + 0*py + (dvx)*pz + (dpz)*vx + 0*vy + (-dpx)*vz = cAy - cBy
		A[rowOffset+1][0] = negRat(dvz)
		A[rowOffset+1][1] = big.NewRat(0, 1)
		A[rowOffset+1][2] = dvx
		A[rowOffset+1][3] = dpz
		A[rowOffset+1][4] = big.NewRat(0, 1)
		A[rowOffset+1][5] = negRat(dpx)
		B[rowOffset+1] = subRat(cAy, cBy)

		// Row 2: Z-component
		// (dvy)*px + (-dvx)*py + 0*pz + (-dpy)*vx + (dpx)*vy + 0*vz = cAz - cBz
		A[rowOffset+2][0] = dvy
		A[rowOffset+2][1] = negRat(dvx)
		A[rowOffset+2][2] = big.NewRat(0, 1)
		A[rowOffset+2][3] = negRat(dpy)
		A[rowOffset+2][4] = dpx
		A[rowOffset+2][5] = big.NewRat(0, 1)
		B[rowOffset+2] = subRat(cAz, cBz)
	}

	addPair(0, h0, h1)
	addPair(3, h0, h2)

	// Gaussian elimination with partial pivoting
	n := 6
	for col := range n {
		pivotRow := -1
		for r := col; r < n; r++ {
			if A[r][col].Num().Sign() != 0 {
				pivotRow = r
				break
			}
		}
		if pivotRow == -1 {
			panic("linear system has no unique solution with selected hailstones")
		}
		if pivotRow != col {
			A[col], A[pivotRow] = A[pivotRow], A[col]
			B[col], B[pivotRow] = B[pivotRow], B[col]
		}

		pivotVal := new(big.Rat).Set(A[col][col])
		for j := col; j < n; j++ {
			A[col][j] = divRat(A[col][j], pivotVal)
		}
		B[col] = divRat(B[col], pivotVal)

		for r := range n {
			if r != col && A[r][col].Num().Sign() != 0 {
				factor := new(big.Rat).Set(A[r][col])
				for j := col; j < n; j++ {
					A[r][j] = subRat(A[r][j], mulRat(factor, A[col][j]))
				}
				B[r] = subRat(B[r], mulRat(factor, B[col]))
			}
		}
	}

	// Solution contains [px, py, pz, vx, vy, vz]
	sum := addRat(addRat(B[0], B[1]), B[2])
	res := new(big.Int).Quo(sum.Num(), sum.Denom()).Int64()
	return res
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

	part2 = solvePart2(input)

	return aoc.Solution{Part1: strconv.FormatInt(part1, 10), Part2: strconv.FormatInt(part2, 10)}
}
