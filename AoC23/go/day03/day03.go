package day03

import (
	"github.com/yury-fedorov/AoC/AoC23/aoc"
	"strconv"
	"unicode"
)

type Day03 struct{}

const empty rune = '.'

func at(engine []string, x, y int) rune {
	if x < 0 || y < 0 || x >= len(engine) || y >= len(engine[0]) {
		return empty
	}
	line := []rune(engine[y])
	return line[x]
}

func isPartialSymbol(ch rune) bool {
	return ch != empty && !unicode.IsDigit(ch)
}

func isPartialNumber(engine []string, startX, endX, y int) bool {
	for dy := -1; dy <= 1; dy++ {
		for xi := startX - 1; xi <= endX; xi++ {
			ch := at(engine, xi, y+dy)
			if isPartialSymbol(ch) {
				return true
			}
		}
	}
	return false
}

func sumPartNumbers(engine []string) int {
	maxY := len(engine)
	maxX := len(engine[0])
	var result int
	for y := 0; y < maxY; y++ {
		const NotStarted int = -1
		var startAt = NotStarted
		line := []rune(engine[y])
		for x := 0; x <= maxX; x++ {
			chi := at(engine, x, y)
			isDigit := unicode.IsDigit(chi)
			if startAt == NotStarted && isDigit {
				startAt = x
			} else if startAt != NotStarted && !isDigit {
				// x - is the index next to the end of the number
				// so the number is around: [startAt, x)
				if isPartialNumber(engine, startAt, x, y) {
					partialNumberText := string(line[startAt:x])
					partialNumber, err := strconv.Atoi(partialNumberText)
					if err != nil {
						panic(partialNumber)
					}
					result += partialNumber
				}
				startAt = NotStarted
			}

		}
	}
	return result
}

func (d Day03) Solve() aoc.Solution {
	var part1, part2 int
	engine := aoc.ReadFile("03")
	part1 = sumPartNumbers(engine)
	part2 = len(engine[0])
	return aoc.Solution{strconv.Itoa(part1), strconv.Itoa(part2)}
}
