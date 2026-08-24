package day21

import (
	"maps"
	"strconv"
	"strings"

	"github.com/yury-fedorov/AoC/AoC23/aoc"
)

type Day21 struct{}

type Point struct{ x, y int }

var Moves = []Point{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}

func (day Day21) Solve() aoc.Solution {
	var part1, part2 int
	m := aoc.ReadFile("21")

	isGardenPlot := func(m []string, p Point) bool {
		if p.x < 0 || p.y < 0 {
			return false
		}
		if p.y >= len(m) {
			return false
		}
		l := []rune(m[p.y])
		if p.x >= len(l) {
			return false
		}
		return l[p.x] != '#'
	}

	var start Point
	for yi, line := range m {
		xi := strings.Index(line, "S")
		if xi >= 0 {
			start = Point{x: xi, y: yi}
			break
		}
	}

	// Part 1: 64 steps
	pp := map[Point]bool{start: true}
	for range 64 {
		pp1 := make(map[Point]bool)
		for p0 := range maps.Keys(pp) {
			for _, dp := range Moves {
				p1 := Point{x: p0.x + dp.x, y: p0.y + dp.y}
				if isGardenPlot(m, p1) {
					pp1[p1] = true
				}
			}
		}
		pp = pp1
	}
	part1 = len(pp)

	// Part 2: 26501365 steps with infinite grid
	// Key insight: 26501365 = 65 + 131 * 202300
	// We need to handle wrapping for the infinite grid
	mapHeight := len(m)
	mapWidth := len([]rune(m[0]))

	isGardenPlotInfinite := func(p Point) bool {
		// Normalize coordinates to fit within the map using modulo
		normX := ((p.x % mapWidth) + mapWidth) % mapWidth
		normY := ((p.y % mapHeight) + mapHeight) % mapHeight
		l := []rune(m[normY])
		return l[normX] != '#'
	}

	// Calculate reachable plots at key step counts for interpolation
	stepCounts := []int{65, 65 + mapWidth, 65 + 2*mapWidth}
	values := make([]int, 3)

	for idx, steps := range stepCounts {
		pp := map[Point]bool{start: true}
		for i := 0; i < steps; i++ {
			pp1 := make(map[Point]bool)
			for p0 := range maps.Keys(pp) {
				for _, dp := range Moves {
					p1 := Point{x: p0.x + dp.x, y: p0.y + dp.y}
					if isGardenPlotInfinite(p1) {
						pp1[p1] = true
					}
				}
			}
			pp = pp1
		}
		values[idx] = len(pp)
	}

	// Quadratic interpolation
	// f(x) = ax^2 + bx + c
	// Solve for a, b, c using the three data points
	// x0, x1, x2 := 0, 1, 2
	y0, y1, y2 := values[0], values[1], values[2]

	// Using Lagrange interpolation or solving the system
	a := (y0 - 2*y1 + y2) / 2
	b := (y1 - y0 - 3*a) / 1
	c := y0

	// Target: 26501365 = 65 + 131 * n, where n = 202300
	n := (26501365 - 65) / mapWidth
	part2 = a*n*n + b*n + c

	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)} // 613385230433078 -- too low
}
