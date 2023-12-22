package day22

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"strconv"
	"strings"
)

type Day22 struct{}

type Point3 struct {
	x, y, z int
}

type Brick struct {
	a, b Point3
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

func (day Day22) Solve() aoc.Solution {
	var part1, part2 int
	bricks := parseBricks(aoc.ReadFile("22"))
	part1 = len(bricks)
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
