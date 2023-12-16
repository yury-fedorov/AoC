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

func moveToNorth(dish Dish) {
	xMax, yMax := size(dish)
	isMoved := true
	for isMoved {
		isMoved = false
		for y := 1; y < yMax; y++ {
			for x := 0; x < xMax; x++ {
				if dish[y][x] == Rock {
					if dish[y-1][x] == Space {
						// move rock one cell up (to north)
						dish[y][x] = Space
						dish[y-1][x] = Rock
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

// test method
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

func (d Day14) Solve() aoc.Solution {
	var part1, part2 int
	dish := parse("14")
	moveToNorth(dish)
	part1 = totalLoad(dish)
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
