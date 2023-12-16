package day11

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"golang.org/x/exp/slices" // Due to 1.20
	"strconv"
)

type Day11 struct{}

type Point struct {
	x, y int
}

const NDValue int = -1

type Image struct {
	Galaxies   []Point
	EmptyLines []Point // NDValue is used on one of coordinates
}

func ParseFile(file string) Image {
	return parse(aoc.ReadFile(file))
}

func parse(data []string) Image {
	const Galaxy rune = '#'
	yMax := len(data)
	xMax := len(data[0])
	var galaxies []Point
	var emptyLine []Point
	for y := 0; y < yMax; y++ {
		line := []rune(data[y])
		isGalaxy := false
		for x := 0; x < xMax; x++ {
			cell := line[x]
			if cell == Galaxy {
				galaxies = append(galaxies, Point{x: x, y: y})
				isGalaxy = true
			}
		}
		if !isGalaxy {
			emptyLine = append(emptyLine, Point{x: NDValue, y: y})
		}
	}
	for x := 0; x < xMax; x++ {
		isGalaxy := false
		for y := 0; y < yMax; y++ {
			cell := []rune(data[y])[x]
			if cell == Galaxy {
				isGalaxy = true
			}
		}
		if !isGalaxy {
			emptyLine = append(emptyLine, Point{x: x, y: NDValue})
		}
	}
	return Image{Galaxies: galaxies, EmptyLines: emptyLine}
}

func emptyLines(image Image, a Point, b Point, isX bool) []int {
	var result []int
	az := aoc.Ifelse(isX, a.x, a.y)
	bz := aoc.Ifelse(isX, b.x, b.y)
	if az > bz {
		az, bz = bz, az
	}
	// now az < bz
	for z := az; z < bz; z++ {
		t := aoc.Ifelse(isX, Point{x: z, y: NDValue}, Point{x: NDValue, y: z})
		if slices.Contains(image.EmptyLines, t) {
			result = append(result, z)
		}
	}
	return result
}

func Distance(image Image, ga Point, gb Point) int {
	dx := aoc.Abs(ga.x - gb.x)
	dy := aoc.Abs(ga.y - gb.y)
	ex := emptyLines(image, ga, gb, true)
	ey := emptyLines(image, ga, gb, false)
	return dx + dy + len(ex) + len(ey)
}

func less(ga Point, gb Point) bool {
	const k int = 1_000
	a := ga.x*k + ga.y
	b := gb.x*k + gb.y
	return a < b
}

func (d Day11) Solve() aoc.Solution {
	var part1, part2 int
	image := ParseFile("11")
	// g := func(i int) Point { return image.Galaxies[i-1] }
	// d := func(a int, b int) int { return Distance(image, g(a), g(b)) }
	// part1 = Distance(image, g(8), g(9)) // 5 right
	// part1 = Distance(image, g(1), g(7)) // 15 right
	// part1 = Distance(image, g(3), g(6)) // 17 right
	for _, ga := range image.Galaxies {
		for _, gb := range image.Galaxies {
			if less(ga, gb) {
				part1 += Distance(image, ga, gb)
				// part2++
			}
		}
	}

	// part1 = len(image.Galaxies)
	// part2 = len(image.EmptyLines)

	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
