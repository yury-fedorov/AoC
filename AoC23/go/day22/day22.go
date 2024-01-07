package day22

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
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

func orderBricks(bricks []*Brick) {
	slices.SortFunc(bricks, func(a, b *Brick) int { return aoc.Compare(a.bottom(), b.bottom()) })
}

func isInAir(b Brick, bricks []*Brick, idToSkip int) bool {
	base := b.bottom()
	// The ground is at z=0 and is perfectly flat; the lowest z value a brick can have is therefore 1.
	if base == 1 {
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

func countInAirDestroyingBrick(brickIdToDestroy int, bricks []*Brick) int {
	var bb []*Brick           // deep copy of bricks
	zMap := make(map[int]int) // id to a.z
	for _, b := range bricks {
		if b.id == brickIdToDestroy {
			continue
		}
		var b1 = *b // copy of the object
		bb = append(bb, &b1)
		zMap[b.id] = b.a.z
	}
	fallDown(bb)
	var result int
	for _, b := range bb {
		result += aoc.Ifelse(b.a.z != zMap[b.id], 1, 0) // if fallen down, then counted
	}
	return result
}

func solution2(bricks []*Brick) int {
	var result int
	for _, b := range bricks {
		result += countInAirDestroyingBrick(b.id, bricks)
	}
	return result
}

func (day Day22) Solve() aoc.Solution {
	bricks := parseBricks(aoc.ReadFile("22"))
	orderBricks(bricks)
	fallDown(bricks)
	return aoc.Solution{strconv.Itoa(solution1(bricks)), strconv.Itoa(solution2(bricks))}
}
