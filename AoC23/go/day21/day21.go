package day21

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"golang.org/x/exp/maps"
	"strconv"
	"strings"
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
	pp := map[Point]bool{start: true}
	for i := 0; i < 64; i++ {
		pp1 := make(map[Point]bool)
		for _, p0 := range maps.Keys(pp) {
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
	const kPart2 = 26501365
	// TODO - map repeats infinitely
	// In exactly 5000 steps, he can reach 16_733_044 garden plots.
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
