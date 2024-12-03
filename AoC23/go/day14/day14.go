package day14

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"strconv"
)

type Day14 struct{}

type Material rune

const (
	Rock           Material = 'O'
	Space          Material = '.'
	CubeShapedRock Material = '#'
)

type Dish [][]Material

func parse(file string) Dish {
	var result [][]Material
	for _, line := range aoc.ReadFile(file) {
		cells := []rune(line)
		var row []Material
		for _, c := range cells {
			row = append(row, Material(c))
		}
		result = append(result, row)
	}
	return result
}

type Direction rune

const (
	North Direction = 'N'
	West  Direction = 'W'
	South Direction = 'S'
	East  Direction = 'E'
)

func moveTo(dish Dish, dir Direction) {
	xMax, yMax := size(dish)
	isMoved := true

	var x0, y0, dx, dy, ox, oy int

	moreX := func(x int) bool { return x < xMax }
	moreY := func(y int) bool { return y < yMax }

	if dir == North {
		y0 = 1
		dx = 1
		dy = 1
		oy = -1
	} else if dir == South {
		y0 = yMax - 2
		dx = 1
		dy = -1
		moreY = func(y int) bool { return y >= 0 }
		oy = 1
	} else if dir == West {
		x0 = 1
		dx = 1
		dy = 1
		ox = -1
	} else { // East
		x0 = xMax - 2
		dx = -1
		dy = 1
		moreX = func(x int) bool { return x >= 0 }
		ox = 1
	}

	for isMoved {
		isMoved = false
		for y := y0; moreY(y); y += dy {
			for x := x0; moreX(x); x += dx {
				if dish[y][x] == Rock {
					if dish[y+oy][x+ox] == Space {
						// move rock one cell in direction
						dish[y][x] = Space
						dish[y+oy][x+ox] = Rock
						isMoved = true
					}
				}
			}
		}
	}
}

func size(dish Dish) (int, int) {
	return len(dish[0]), len(dish)
}

func totalLoad(dish Dish) int {
	xMax, yMax := size(dish)
	var result int
	for y := 0; y < yMax; y++ {
		for x := 0; x < xMax; x++ {
			if dish[y][x] == Rock {
				result += yMax - y
			}
		}
	}
	return result
}

func show(dish Dish) string {
	xMax, yMax := size(dish)
	var result string
	for y := 0; y < yMax; y++ {
		for x := 0; x < xMax; x++ {
			result += string(dish[y][x])
		}
		result += "\n"
	}
	return result
}

func (day Day14) Solve() aoc.Solution {
	var part1, part2 int
	file := "14"
	dish := parse(file)
	moveTo(dish, North)
	part1 = totalLoad(dish)

	const circleCount int = 1_000_000_000
	dish = parse(file)
	spinCircle := []Direction{North, West, South, East}
	cache := make(map[string]int)
	for i := 0; i < circleCount; i++ {
		for _, dir := range spinCircle {
			moveTo(dish, dir)
		}
		key := show(dish)
		prev, found := cache[key]
		if found {
			// now we know the circle
			circleSize := i - prev
			k := (circleCount - i + 1) / circleSize
			i += k * circleSize
		} else {
			cache[key] = i
		}
	}
	part2 = totalLoad(dish)
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
