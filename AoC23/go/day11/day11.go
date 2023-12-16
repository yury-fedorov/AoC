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

func emptyLines(image Image, ga Point, gb Point, isX bool) []int {
	var result []int
	gaz := aoc.Ifelse(isX, ga.x, ga.y)
	gbz := aoc.Ifelse(isX, gb.x, ga.y)
	if gaz > gbz {
		gaz, gbz = gbz, gaz
	}
	// now gaz < gbz
	for z := gaz; z < gbz; z++ {
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
	image := ParseFile("11-1")
	for _, ga := range image.Galaxies {
		for _, gb := range image.Galaxies {
			if less(ga, gb) {
				part1 += Distance(image, ga, gb)
				part2++
			}
		}
	}
	// part1 = len(image.Galaxies)
	// part2 = len(image.EmptyLines)

	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
