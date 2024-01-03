package day22

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"golang.org/x/exp/maps"
	"strconv"
	"strings"
)

type Day22 struct{}

type Point struct {
	x, y int
}

type Point3 struct {
	x, y, z int
}

type Brick struct {
	a, b Point3
}

func brickToCubes(b Brick) []Point3 {
	var result []Point3
	// XXX: not real mod, if 0 - exit from the loop with a big increment (no 1 mln in input)
	mod := func(a int) int { return aoc.Ifelse(a == 0, 1_000_000, aoc.Ifelse(a > 0, 1, -1)) }
	for x := b.a.x; x <= b.b.x; x += mod(b.b.x - b.a.x) {
		for y := b.a.y; y <= b.b.y; y += mod(b.b.y - b.a.y) {
			for z := b.a.z; z <= b.b.z; z += mod(b.b.z - b.a.z) {
				result = append(result, Point3{x, y, z})
			}
		}
	}
	return result
}

func parseBricks(data []string) []Brick {
	var result []Brick
	pp := func(p string) Point3 {
		a := aoc.ToSlice(strings.ReplaceAll(p, ",", " "))
		return Point3{x: int(a[0]), y: int(a[1]), z: int(a[2])}
	}
	for _, line := range data {
		points := strings.Split(line, "~")
		result = append(result, Brick{pp(points[0]), pp(points[1])})
	}
	return result
}

// The ground is at z=0 and is perfectly flat; the lowest z value a brick can have is therefore 1.
func isOnGround(b Brick) bool {
	return aoc.Min(b.a.z, b.b.z) <= 1
}

// all points covered on z projection
func zProjection(bb []Brick) []Point {
	m := make(map[Point]bool)
	for _, b := range bb {
		for _, p3 := range brickToCubes(b) {
			m[Point{p3.x, p3.y}] = true
		}
	}
	return maps.Keys(m)
}

func (day Day22) Solve() aoc.Solution {
	var part1, part2 int
	// bricks := parseBricks(aoc.ReadFile("22-1"))
	// part1 = len(zProjection(bricks))
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
