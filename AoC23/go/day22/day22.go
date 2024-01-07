package day22

import (
	"cmp"
	"fmt"
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"golang.org/x/exp/maps"
	"golang.org/x/exp/slices"
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
	id   int
	a, b Point3
}

func (brick *Brick) bottom() int { return aoc.Min(brick.a.z, brick.b.z) }
func (brick *Brick) top() int    { return aoc.Max(brick.a.z, brick.b.z) }

func (brick *Brick) fallDown(dz int) {
	brick.a.z -= dz
	brick.b.z -= dz
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

func parseBricks(data []string) []*Brick {
	var result []*Brick
	pp := func(p string) Point3 {
		a := aoc.ToSlice(strings.ReplaceAll(p, ",", " "))
		return Point3{x: int(a[0]), y: int(a[1]), z: int(a[2])}
	}
	id := 0
	for _, line := range data {
		points := strings.Split(line, "~")
		result = append(result, &Brick{id: id, a: pp(points[0]), b: pp(points[1])})
		id++
	}
	return result
}

// The ground is at z=0 and is perfectly flat; the lowest z value a brick can have is therefore 1.
func isOnGround(b Brick) bool {
	return aoc.Min(b.a.z, b.b.z) <= 1
}

func zBrickProjection(b Brick) []Point {
	var result []Point
	for _, p3 := range brickToCubes(b) {
		result = append(result, Point{p3.x, p3.y})
	}
	return result
}

func areOverlapping(a, b []Point) bool {
	for _, ai := range a {
		if slices.Contains(b, ai) {
			return true
		}
	}
	return false
}

// all points covered on z projection
func zProjection(bb []Brick) []Point {
	m := make(map[Point]bool)
	for _, b := range bb {
		for _, p3 := range zBrickProjection(b) {
			m[Point{p3.x, p3.y}] = true
		}
	}
	return maps.Keys(m)
}

func orderBricks(bricks []*Brick) {
	slices.SortFunc(bricks, func(a, b *Brick) int { return cmp.Compare(a.bottom(), b.bottom()) })
}

const lowestAboveGround = 1

func isInAir(b Brick, bricks []*Brick, idToSkip int) bool {
	base := b.bottom()
	if base == lowestAboveGround {
		return false
	}
	baseZProjection := zBrickProjection(b)
	for _, bi := range bricks {
		if bi.id == idToSkip {
			continue
		}
		if (*bi).top() == (base-1) && bi.id != b.id {
			if areOverlapping(baseZProjection, zBrickProjection(*bi)) {
				return false
			}
		}
	}
	return true
}
func fallDown(bricks []*Brick) {
	for {
		isMoved := false
		for _, b := range bricks {
			if isInAir(*b, bricks, b.id) {
				b.fallDown(1)
				isMoved = true
			}
		}
		if !isMoved {
			break
		}
	}
}

func solution1(bricks []*Brick) int {
	var result int
	for _, b := range bricks {
		isMoved := false
		for _, bi := range bricks {
			if b.id == bi.id {
				continue
			}
			if isInAir(*bi, bricks, b.id) {
				isMoved = true
				break
			}
		}
		result += aoc.Ifelse(isMoved, 0, 1)
	}
	return result
}

func (day Day22) Solve() aoc.Solution {
	var part1, part2 int
	bricks := parseBricks(aoc.ReadFile("22"))
	// the lowest z value a brick can have is therefore 1
	// part1 = len(zProjection(bricks))
	orderBricks(bricks)
	fallDown(bricks)
	for _, b := range bricks {
		if isInAir(*b, bricks, b.id) {
			fmt.Println(b)
		}
	}
	part1 = solution1(bricks)
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
